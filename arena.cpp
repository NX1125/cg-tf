
#include "arena.h"
#include "imageloader.h"

arena_t::arena_t(float height, float radius, wf_object_loader_t& loader) :
height(height), radius(radius) {
    blueprint = new circle_blueprint_t(32);
    radiusSqr = radius * radius;

    ground = loader.loadRes("ground-test");

    // roofTexture = loadTextureRAW("models/vista.bmp");
    groundTexture = loadTextureRAW("models/grama.bmp");
    roofTexture = loadTextureRAW("models/ceu.bmp");
    wallTexture = loadTextureRAW("models/vista.bmp");
}

void arena_t::draw() {
    glPushMatrix();
    {
        airstrip->draw();
        glScalef(radius, radius, 1);
        drawGround();
        drawRoof();

        drawWalls();
    }
    glPopMatrix();
}

void arena_t::drawGround() {
    GLfloat color[] = {0x66 / 255.0f, 0x3D / 255.0f, 0x14 / 255.0f, 1.0f}; // brown

    glColor4fv(color);
    //    glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
    //    glMaterialfv(GL_FRONT, GL_SPECULAR, color);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, groundTexture);
    // to draw the ground, we just need to draw the circle itself
    blueprint->draw(true);
    glDisable(GL_TEXTURE_2D);
    //    glRotatef(90, 1,0,0);
    //    ground->draw();
}

void arena_t::drawRoof() {
    glColor3f(0, 0, 1); // blue
    glTranslatef(0, 0, height);
    glRotatef(180, 1, 0, 0);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, roofTexture);
    blueprint->draw(true);
    glDisable(GL_TEXTURE_2D);
}

void arena_t::drawWall(int i1, int i2) {
    vector2d p1 = blueprint->getPoint(i1);
    vector2d p2 = blueprint->getPoint(i2);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, wallTexture);
    glBegin(GL_QUADS);
    {
        glTexCoord2f(1, 1);
        glVertex3f(p1.x, p1.y, 0);
        glTexCoord2f(1, 0);
        glVertex3f(p1.x, p1.y, height);
        glTexCoord2f(0, 0);
        glVertex3f(p2.x, p2.y, height);
        glTexCoord2f(0, 1);
        glVertex3f(p2.x, p2.y, 0);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void arena_t::drawWalls() {
    for (int i = 1; i < blueprint->numOfPoints; i++) {
        drawWall(i - 1, i );
    }
    drawWall(blueprint->numOfPoints - 1, 0);
}

void arena_t::drawMapElement(circle_blueprint_t* blueprint) const {
    // make the outline of the arena as white
    glColor3f(1, 1, 1);
    // Draw the outline with the radius of the arena.
    blueprint->draw(false, radius);
}
