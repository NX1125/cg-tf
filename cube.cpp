
#include <GL/gl.h>

#include "cube.h"

GLfloat cube_t::normals[6][3] = {/* Normals for the 6 faces of a cube. */
    {-1.0, 0.0, 0.0},
    {0.0, 1.0, 0.0},
    {1.0, 0.0, 0.0},
    {0.0, -1.0, 0.0},
    {0.0, 0.0, 1.0},
    {0.0, 0.0, -1.0}
};
GLint cube_t::faces[6][4] = {/* Vertex indices for the 6 faces of a cube. */
    {0, 1, 2, 3},
    {3, 2, 6, 7},
    {7, 6, 5, 4},
    {4, 5, 1, 0},
    {5, 6, 2, 1},
    {7, 4, 0, 3}
};
GLfloat cube_t::vertices[8][3]; /* Will be filled in with X,Y,Z vertexes. */
GLfloat cube_t::color[6][3] = {
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1},
    {1, 1, 0},
    {0, 1, 1},
    {1, 0, 1},
};

bool cube_t::sStart = init();

bool cube_t::init() {
    vertices[0][0] = vertices[1][0] = vertices[2][0] = vertices[3][0] = -1;
    vertices[4][0] = vertices[5][0] = vertices[6][0] = vertices[7][0] = 1;
    vertices[0][1] = vertices[1][1] = vertices[4][1] = vertices[5][1] = -1;
    vertices[2][1] = vertices[3][1] = vertices[6][1] = vertices[7][1] = 1;
    vertices[0][2] = vertices[3][2] = vertices[4][2] = vertices[7][2] = 1;
    vertices[1][2] = vertices[2][2] = vertices[5][2] = vertices[6][2] = -1;
}

void cube_t::drawBox() {
    int i;

    for (i = 0; i < 6; i++) {
        glColor3f(color[i][0], color[i][1], color[i][2]);
        glBegin(GL_QUADS);
        glNormal3fv(&normals[i][0]);
        glVertex3fv(&vertices[faces[i][0]][0]);
        glVertex3fv(&vertices[faces[i][1]][0]);
        glVertex3fv(&vertices[faces[i][2]][0]);
        glVertex3fv(&vertices[faces[i][3]][0]);
        glEnd();
    }
}
