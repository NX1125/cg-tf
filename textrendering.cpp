
#include "textrendering.h"

#include <GL/gl.h>
#include <GL/glut.h>
#include <cstring>

void measureText(const char* text, int* width, int* height) {
    *width = glutBitmapLength(GLUT_BITMAP_9_BY_15, (const unsigned char*) text);
    *height = 15;
}

void drawText(const char* text) {
    //Push to recover original attributes
    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    //Draw text in the x, y, z position
    glRasterPos3f(0, 0, 0);
    while (*text) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *text);
        text++;
    }
    glPopAttrib();
}
