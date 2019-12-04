
//#define _USE_MATH_DEFINES

#include <math.h>
#include <cstring>
#include <cstdio>

#include "shapes.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifndef M_TWOPI 
#define M_TWOPI (2 * M_PI)
#endif

circle_blueprint::circle_blueprint(double radius, double maxArcLength) :
circle_blueprint((unsigned int) ceil((M_TWOPI * radius) / maxArcLength)) {
}

circle_blueprint::circle_blueprint(unsigned int n) : numOfPoints(n) {
    points = new vector2d[n];

    double angle = M_TWOPI / n;

    for (unsigned int i = 0; i < n; i++) {
        struct vector2d v = {
            cos(i * angle),
            sin(i * angle),
        };

        points[i] = v;

        // printf("(%f, %f)\n", v.x, v.y);
    }
}

void circle_blueprint::draw(bool opaque) {
    glBegin(opaque ? GL_TRIANGLE_FAN : GL_LINE_LOOP);
    {
        if (opaque) {
            // makes all of the following points to
            // fan out of the center.
            glVertex2d(0, 0);
        }
        for (int i = 0; i < numOfPoints; i++) {
            const vector2d& v = points[i];
            glVertex2d(v.x, v.y);
        }
        if (opaque) {
            // go back to the first vector to connect the last points.
            const vector2d& v = points[0];
            glVertex2d(v.x, v.y);
        }
    }
    glEnd();
}

circle_blueprint::~circle_blueprint() {
    if (points != NULL) {
        delete points;
    }
}

circle::~circle() {
    delete blueprint;
}

void circle::draw(bool opaque) {
    glPushMatrix();
    {
        translate();
        // printf("scale = %f\n", radius);
        glScaled(radius, radius, 1);
        blueprint->draw(opaque);
    }
    glPopMatrix();
}

void circle::translate() {
    glTranslated(cx, cy, 0);
}

void circle_blueprint::draw(bool opaque, double r) {
    glScaled(r, r, 1);
    draw(opaque);
}
