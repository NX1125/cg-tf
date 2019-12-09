
#include "arena.h"

arena_t::arena_t(float height, float radius, wf_object_loader_t& loader) :
height(height), radius(radius) {
    blueprint = new circle_blueprint_t(32);
    radiusSqr = radius * radius;

    ground = loader.loadRes("ground-test");
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
    GLfloat color[] = {0x66 / 255.0f, 0x3D / 255.0f, 0x14 / 255.0f, 1.0f}; // brown

    glColor4fv(color);
    //    glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
    //    glMaterialfv(GL_FRONT, GL_SPECULAR, color);

    // to draw the ground, we just need to draw the circle itself
    blueprint->draw(true);
    //    glRotatef(90, 1,0,0);
    //    ground->draw();
}

void arena_t::drawRoof() {
    glColor3f(0, 0, 1); // blue
    glTranslatef(0, 0, height);
    glRotatef(180, 1, 0, 0);
    blueprint->draw(true);
}

void arena_t::drawMapElement(circle_blueprint_t* blueprint) const {
    // make the outline of the arena as white
    glColor3f(1, 1, 1);
    // Draw the outline with the radius of the arena.
    blueprint->draw(false, radius);
}
