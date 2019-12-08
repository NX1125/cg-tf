
#include "arena.h"

arena_t::arena_t(float height, float radius) :
height(height), radius(radius) {
    blueprint = new circle_blueprint_t(32);
    radiusSqr = radius * radius;
}

void arena_t::draw() {
    glPushMatrix();
    {
        airstrip->draw();
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

void arena_t::drawMapElement(circle_blueprint_t* blueprint) const {
    // make the outline of the arena as white
    glColor3f(1,1,1);
    // Draw the outline with the radius of the arena.
    blueprint->draw(false, radius);
}
