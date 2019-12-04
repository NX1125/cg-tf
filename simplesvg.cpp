
#include <complex>
#include <GL/gl.h>

#include "simplesvg.h"
// #include "config/defs.h"

#define ARC_LENGTH 5

void simple_svg_line::debug() {
    printf("(%f, %f) --> (%f, %f)\n", x1, y1, x2, y2);
}

simple_svg::simple_svg(const char* filename) {
    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        throw IOException(string("could not find ") + string(filename));
    }

    try {
        TiXmlDocument doc;
        if (doc.LoadFile(file)) {
            TiXmlElement* root = doc.FirstChildElement();
            for (TiXmlElement* e = root->FirstChildElement();
                    e != NULL;
                    e = e->NextSiblingElement()) {
                string name(e->Value());
                if (name == "circle") {
                    circles.push_back(simple_svg_circle(e));
                    shapes.push_back(&circles[circles.size() - 1]);
                } else if (name == "rect") {
                    shapes.push_back(new simple_svg_rect(e));
                } else if (name == "ellipse") {
                    shapes.push_back(new simple_svg_ellipse(e));
                } else if (name == "path") {
                    shapes.push_back(new simple_svg_path(e));
                } else if (name == "line") {
                    lines.push_back(simple_svg_line(e));
                    shapes.push_back(&lines[lines.size() - 1]);
                } else {
                    printf("[WARNING] The element <%s> will be ignored from the SVG file\n", name.c_str());
                }
            }
            fclose(file);
        } else {
            throw IOException(doc.ErrorDesc());
        }
    } catch (...) {
        if (file != NULL) {
            fclose(file);
        }

        throw;
    }
}

void svgMakeIdentity(simple_svg_matrix* m) {
    // remember that this is column-major! Since it is identity, it is equal to 
    // its transpose.
    const simple_svg_matrix m2 = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1,
    };
    memcpy(m, &m2, sizeof (simple_svg_matrix));
}

simple_svg_shape::simple_svg_shape(const TiXmlElement* root) {
    fillName = readAttribute(root, "fill");

    if (fillName == "black") {
        fillColor = {0, 0, 0};
    } else if (fillName == "orange") {
        fillColor = {0xFF / 255.0, 0xA5 / 255.0, 0x00 / 255.0};
    } else if (fillName == "blue") {
        fillColor = {0, 0, 1};
    } else if (fillName == "green") {
        fillColor = {0, 1, 0};
    } else if (fillName == "red") {
        fillColor = {1, 0, 0};
    }

    const char* value = root->Attribute("transform");

    svgMakeIdentity(&transform);

    if (value != NULL) {
        float angle;
        if (sscanf(value, "matrix(%f,%f,%f,%f,%f,%f)",
                &transform[0],
                &transform[1],
                &transform[4],
                &transform[5],
                &transform[12],
                &transform[13]) == 6) {
            // printf("=================== matrix\n");
        } else if (sscanf(value, "scale(%f,%f)",
                &transform[0],
                &transform[5]) == 2) {
            // printf("=================== scale\n");
        } else if (sscanf(value, "rotate(%f)", &angle) == 1) {
            // printf("=================== rotate\n");
            double c = cos(angle);
            double s = sin(angle);

            transform[0] = c;
            transform[5] = c;

            transform[1] = s;
            transform[4] = -s;
        }
    }
    
    value = root->Attribute("id");
    
    if (value != NULL) {
        id = string(value);
    }
}

simple_svg_circle::simple_svg_circle(const TiXmlElement* root) : simple_svg_shape(root) {
    cx = readDoubleAttribute(root, "cx");
    cy = readDoubleAttribute(root, "cy");

    radius = readDoubleAttribute(root, "r");
}

circle_blueprint* simple_svg_ellipse::blueprint = NULL;

simple_svg_ellipse::simple_svg_ellipse(const TiXmlElement* root) : simple_svg_shape(root) {
    cx = readDoubleAttribute(root, "cx");
    cy = readDoubleAttribute(root, "cy");

    rx = readDoubleAttribute(root, "rx");
    ry = readDoubleAttribute(root, "ry");

    if (blueprint == NULL) {
        blueprint = new circle_blueprint(64);
    }
}

simple_svg_rect::simple_svg_rect(const TiXmlElement* root) : simple_svg_shape(root) {
    x = readDoubleAttribute(root, "x");
    y = readDoubleAttribute(root, "y");

    width = readDoubleAttribute(root, "width");
    height = readDoubleAttribute(root, "height");
}

simple_svg_line::simple_svg_line(const TiXmlElement* root) {
    x1 = readDoubleAttribute(root, "x1");
    y1 = readDoubleAttribute(root, "y1");
    x2 = readDoubleAttribute(root, "x2");
    y2 = readDoubleAttribute(root, "y2");
}

void simple_svg_circle::toCircle(Circle dest) const {
    dest->cx = cx;
    dest->cy = cy;
    dest->radius = radius;
    dest->blueprint = new circle_blueprint(radius, ARC_LENGTH);
}

void simple_svg_line::toLine(line* line) const {
    line->x1 = x1;
    line->y1 = y1;
    line->x2 = x2;
    line->y2 = y2;
}

void simple_svg_circle::onDraw() {
    if (blueprint != NULL) {
        glTranslated(cx, cy, 0);
        glScaled(radius, radius, 1);
        setFillToGL();
        transformGL();
        blueprint->draw(true);
    }
}

void simple_svg_ellipse::onDraw() {
    if (blueprint != NULL) {
        glTranslated(cx, cy, 0);
        glScaled(rx, ry, 1);
        setFillToGL();
        transformGL();
        blueprint->draw(true);
    }
}

void simple_svg::draw(int i) {
    shapes[i]->draw();
}

void simple_svg::draw() {
    for (unsigned int i = 0; i < shapes.size(); i++)
        shapes[i]->draw();
}

void simple_svg_line::onDraw() {
    setFillToGL();
    glBegin(GL_LINES);
    glVertex2d(x1, y1);
    glVertex2d(x2, y2);
    glEnd();
}

void simple_svg_rect::onDraw() {
    setFillToGL();
    glTranslated(x, y, 0);
    transformGL();
    glBegin(GL_TRIANGLE_FAN);
    glVertex2d(0, 0);
    glVertex2d(width, 0);
    glVertex2d(width, height);
    glVertex2d(0, height);
    glEnd();
}

bool isDigit(char c) {
    return '0' <= c && c <= '9';
}

simple_svg_path::simple_svg_path(const TiXmlElement* root) : simple_svg_shape(root) {
    // we have a sequence of floats to read here.
    // This is a parser without design.

    path = NULL;

    int capacity = 0;

    const char* attr = readAttribute(root, "d");

    while (*attr) {
        if (isDigit(*attr) || *attr == '.' || *attr == '+' || *attr == '-') {
            // parse number
            if (n + 1 > capacity) {
                capacity = n + 16;
                path = (double*) realloc(path, capacity * sizeof (double));
            }
            sscanf(attr, "%lf", path + n);
            n++;
            // skip number
            while (*attr != '\0' && (isDigit(*attr) || *attr == '.' || *attr == '+' || *attr == '-')) {
                attr++;
            }
            continue;
        }
        attr++;
    }

    printf("There are %d points\n", n);

    strip = (double*) malloc(2 * n * sizeof (double));

    n /= 2;
}

void simple_svg_path::onDraw() {
    setFillToGL();
    glBegin(GL_TRIANGLE_STRIP);
    int i = 0;
    int k = n * 2;
    
    while (i < k) {
        glVertex2f(path[i], path[i + 1]);
        glVertex2f(path[k - 2], path[k - 1]);
        
        k-=2;
        i+=2;
    }
    
    glEnd();
}

