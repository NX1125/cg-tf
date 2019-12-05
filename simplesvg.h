/* 
 * File:   simplesvg.h
 * Author: guilh
 *
 * Created on 3 de outubro de 2019, 12:44
 */

#ifndef SIMPLESVG_H
#define SIMPLESVG_H

#include <vector>
#include <stdio.h>

#include "xmlutils.h"
#include "shapes.h"
// #include "config/defs.h"

#define DEPRECATED

using namespace std;

struct simple_svg_color {
    float r;
    float g;
    float b;
};

typedef GLfloat simple_svg_matrix[16];

void svgMakeIdentity(simple_svg_matrix* m);

class simple_svg_shape {
public:
    string fillName;

    simple_svg_matrix transform;
    
    string id;

    simple_svg_color fillColor;

    simple_svg_shape(const TiXmlElement* root);

    simple_svg_shape() {
        fillName = "black";

        svgMakeIdentity(&transform);
    }

    virtual ~simple_svg_shape() {
    }

    void setFillToGL() {
        glColor3f(fillColor.r, fillColor.g, fillColor.b);
    }

    virtual void translate(float dx, float dy) = 0;

    void transformGL() {
        // simple_svg_matrix matrix;
        // glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
        // glLoadMatrixf(transform);
        // glMultMatrixf(matrix);

        glMultMatrixf(transform);
    }

    void draw() {
        glPushMatrix();
        {
            onDraw();
        }
        glPopMatrix();
    }

protected:
    virtual void onDraw() = 0;
};

class simple_svg_circle : public simple_svg_shape {
public:
    double cx;
    double cy;

    double radius;

    circle_blueprint_t* blueprint = NULL;

    simple_svg_circle(const TiXmlElement* root);

    DEPRECATED struct circle toCircle() const;

    void toCircle(Circle dest) const;

    void onDraw() override;

    void translate(float dx, float dy) override {
        cx += dx;
        cy += dy;
    }

};

class simple_svg_ellipse : public simple_svg_shape {
private:
    static circle_blueprint_t* blueprint;

public:
    double cx;
    double cy;

    double rx;
    double ry;

    simple_svg_ellipse(const TiXmlElement* root);

    void onDraw() override;

    void translate(float dx, float dy) override {
        cx += dx;
        cy += dy;
    }

};

class simple_svg_rect : public simple_svg_shape {
public:
    double x;
    double y;

    double width;
    double height;

    simple_svg_rect(const TiXmlElement* root);

    void onDraw() override;

    void translate(float dx, float dy) override {
        x += dx;
        y += dy;
    }

};

class simple_svg_path : public simple_svg_shape {
public:
    double* path = NULL;
    double* strip = NULL;
    
    int n = 0;

    simple_svg_path(const TiXmlElement* root);

    void onDraw() override;

    void translate(float dx, float dy) override {
        for (int i = 0; i < n; i++) {
            path[2 * i] += dx;
            path[2 * i + 1] += dy;
            
            strip[2 * i] += dx;
            strip[2 * i + 1] += dy;
        }
    }
};

class simple_svg_line : public simple_svg_shape {
public:
    double x1;
    double y1;

    double x2;
    double y2;

    simple_svg_line(const TiXmlElement* root);

    void debug();

    DEPRECATED struct line toLine() const;

    void toLine(struct line* line) const;

    void onDraw() override;

    void translate(float dx, float dy) override {
        x1 += dx;
        y1 += dy;
        x2 += dx;
        y2 += dy;
    }
};

struct simple_svg {
    std::vector<simple_svg_circle> circles;
    std::vector<simple_svg_line> lines;

    std::vector<simple_svg_shape*> shapes;

    simple_svg(const char* filename);

    unsigned int getNumOfShapes() {
        return shapes.size();
    }

    void draw(int i);
    void draw();
};

#endif /* SIMPLESVG_H */

