
#include "shapes.h"

#include <GL/gl.h>

void line::draw() {
    glBegin(GL_LINES);
    glVertex2d(x1, y1);
    glVertex2d(x2, y2);
    glEnd();
}
