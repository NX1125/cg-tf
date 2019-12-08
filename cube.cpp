
#include <GL/gl.h>

#include "cube.h"

// https://www.opengl.org/archives/resources/code/samples/glut_examples/examples/cube.c

void drawCubeX(GLfloat x) {
    glBegin(GL_QUADS);
    {
        glNormal3f(x, 0, 0);

        glVertex3f(x, -1, -1);
        glVertex3f(x, +1, -1);
        glVertex3f(x, +1, +1);
        glVertex3f(x, -1, +1);
    }
    glEnd();
}

void drawCubeY(GLfloat y) {
    glBegin(GL_QUADS);
    {
        glNormal3f(0, y, 0);

        glVertex3f(+1, y, -1);
        glVertex3f(-1, y, -1);
        glVertex3f(-1, y, +1);
        glVertex3f(+1, y, +1);
    }
    glEnd();
}

void drawCubeZ(GLfloat z) {
    glBegin(GL_QUADS);
    {
        glNormal3f(0, 0, z);
        glVertex3f(-1, -1, z);
        glVertex3f(+1, -1, z);
        glVertex3f(+1, +1, z);
        glVertex3f(-1, +1, z);
    }
    glEnd();
}

void drawCube(void) {
    glColor3f(1, 0, 0);
    glCullFace(GL_BACK);
    drawCubeX(1);

    glColor3f(0, 1, 1);
    glCullFace(GL_FRONT);
    drawCubeX(-1);

    glColor3f(0, 1, 0);
    glCullFace(GL_BACK);
    drawCubeY(1);

    glColor3f(1, 0, 1);
    glCullFace(GL_FRONT);
    drawCubeY(-1);

    glColor3f(0, 0, 1);
    glCullFace(GL_BACK);
    drawCubeZ(1);

    glColor3f(1, 1, 0);
    glCullFace(GL_FRONT);
    drawCubeZ(-1);

    glCullFace(GL_BACK);
}
