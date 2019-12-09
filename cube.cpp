
#include <GL/gl.h>
#include <GL/freeglut_std.h>

#include "cube.h"
#include "wfobj.h"

// https://www.opengl.org/archives/resources/code/samples/glut_examples/examples/cube.c

wf_object_t* sCubeModel = NULL;

void loadCube(wf_object_loader_t& loader) {
    sCubeModel = loader.loadRes("cube-test");
}

void drawCubeX(GLfloat r, GLfloat g, GLfloat b) {
    GLfloat mcolor[] = {r, g, b, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mcolor);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mcolor);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50.0);

    glBegin(GL_QUADS);
    {
        glNormal3f(1, 0, 0);

        glVertex3f(1, -1, -1);
        glVertex3f(1, +1, -1);
        glVertex3f(1, +1, +1);
        glVertex3f(1, -1, +1);
    }
    glEnd();
}

void drawCubeY(GLfloat r, GLfloat g, GLfloat b) {
    GLfloat mcolor[] = {r, g, b, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mcolor);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mcolor);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50.0);

    glBegin(GL_QUADS);
    {
        glNormal3f(0, 1, 0);

        glVertex3f(+1, 1, -1);
        glVertex3f(-1, 1, -1);
        glVertex3f(-1, 1, +1);
        glVertex3f(+1, 1, +1);
    }
    glEnd();
}

void drawCubeZ(GLfloat r, GLfloat g, GLfloat b) {
    GLfloat mcolor[] = {r, g, b, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mcolor);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mcolor);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50.0);

    glBegin(GL_QUADS);
    {
        glNormal3f(0, 0, 1);
        glVertex3f(-1, -1, 1);
        glVertex3f(+1, -1, 1);
        glVertex3f(+1, +1, 1);
        glVertex3f(-1, +1, 1);
    }
    glEnd();
}

void drawCube(void) {
    // GLfloat mcolor[] = {1, 0, 0, 1.0};
    // glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mcolor);
    // glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mcolor);
    // glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50.0);

    sCubeModel->draw();

    //        glutSolidCube(2);

    //    glPushMatrix();
    //    {
    //        drawCubeX(1, 0, 0);
    //        glRotatef(180, 0, 1, 0);
    //        drawCubeX(0, 1, 1);
    //    }
    //    glPopMatrix();
    //
    //    glPushMatrix();
    //    {
    //        drawCubeY(0, 1, 0);
    //        glRotatef(180, 1, 0, 0);
    //        drawCubeY(1, 0, 1);
    //    }
    //    glPopMatrix();
    //
    //    glPushMatrix();
    //    {
    //        drawCubeZ(0, 0, 1);
    //        glRotatef(180, 0, 1, 0);
    //        drawCubeZ(1, 1, 0);
    //    }
    //    glPopMatrix();
}
