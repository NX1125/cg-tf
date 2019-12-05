
#include "arena.h"

void arena_t::draw() {
    glPushMatrix();
    {
        glScalef(radius, radius, 1);
        drawGround();
        drawRoof();
    }
    glPopMatrix();
}

void arena_t::drawGround() {
    glColor3f(0x66 / 255.0f, 0x3D / 255.0f, 0x14 / 255.0f); // brown
    // to draw the ground, we just need to draw the circle itself
    blueprint->draw(true);
}

void arena_t::drawRoof() {
    glColor3f(0, 0, 1); // blue
    glCullFace(GL_FRONT);
    glTranslatef(0, 0, height);
    blueprint->draw(true);
    glCullFace(GL_BACK);
}
