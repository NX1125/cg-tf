
#include "textrendering.h"

#include <GL/gl.h>
#include <GL/glut.h>
#include <cstring>

void measureText(const unsigned char* text, int* width, int* height) {
    *width = glutBitmapLength(GLUT_BITMAP_9_BY_15, text);
    *height = 15;
}

void rasterChars(GLfloat x, GLfloat y, GLfloat z, const char* text, double r, double g, double b) {
    //Push to recover original attributes
    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    //Draw text in the x, y, z position
    glColor3f(r, g, b);
    glRasterPos3f(x, y, z);
    while (*text) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *text);
        text++;
    }
    glPopAttrib();
}

void drawText(GLfloat x, GLfloat y, const char * text, double r, double g, double b) {
    //Draw text considering a 2D space (disable all 3d features)
    glMatrixMode(GL_PROJECTION);
    //Push to recover original PROJECTION MATRIX
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, 1, 0, 1, -1, 1);
    // glScaled(10,10,1);
    rasterChars(x, y, 0, text, r, g, b);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}