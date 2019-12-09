
/* 
 * File:   flyingenemy.cpp
 * Author: 2019202344
 * 
 * Created on 6 de Dezembro de 2019, 18:22
 */

#include <stddef.h>
#include <math.h>
#include <GL/gl.h>

#include "flyingenemy.h"
#include "shapes.h"
#include "cube.h"

wf_object_t* flying_enemy_t::sEnemyModel = NULL;
vector3f flying_enemy_t::sCannonOffset(1, 0, -1);

std::default_random_engine flying_enemy_t::sRandomMovement;

flying_enemy_t::flying_enemy_t(point3f position, float radius) :
radius(radius) {
    controller = new airplane_movement_t();
    controller->setPosition(position);

    propeller = new propeller_t(vector3f(0, 0, radius * 1.1f));
}

void flying_enemy_t::setInitialVelocity(float initialVelocity) {
    this->initialVelocity = initialVelocity;
    controller->setMagnitude(initialVelocity);
}

void flying_enemy_t::update(int millis) {
    if (dead) return;

    // TODO Fire bullet with a frequency defined in the settings

    // TODO Make the enemy follow the player instead
    accumulatedTime += millis;
    if (accumulatedTime > 1000) {
        accumulatedTime = 0;
        switch (behaviour) {
            case 0:
                controller->setInputAxis(1, 0);
                break;
            case 1:
                controller->setInputAxis(-1, 0);
                break;
            case 2:
                // controller->setInputAxis(0, -controller->getVerticalAngle() * 0.01f);
                break;
            case 3:
                // controller->setInputAxis(0.01f, 0.01f);
                break;
            default:
                behaviour = -1;
        }
        behaviour++;
    }
    controller->update(millis);


    const float k = 0.95f;

    horizontal *= k;
    horizontal += controller->getHorizontalAngularVelocity() * (1 - k);

    propeller->setMagnitude(controller->getMagnitude());
    propeller->update(millis);
}

void flying_enemy_t::clipZ(float z) {
    controller->clipZ(z, radius);
}

point3f flying_enemy_t::getPosition() const {
    return controller->getPosition();
}

float flying_enemy_t::getRadius() const {
    return radius;
}

void flying_enemy_t::kill() {
    dead = true;
}

void flying_enemy_t::transformAndDraw() {
    // TODO Draw cannon of enemy
    // TODO Add propeller to enemy

    if (dead) return;
    glPushMatrix();
    {
        point3f p = controller->getPosition();
        glTranslatef(p.x, p.y, p.z);
        drawAxis(radius);
        // considering that the front of the airplane is at +x and the back is at -x
        const float degreePerRadians = 180 / M_PI;
        glRotatef(controller->getHorizontalAngle() * degreePerRadians, 0, 0, 1);
        glRotatef(controller->getVerticalAngle() * degreePerRadians, 0, 1, 0);
        glRotatef(horizontal * degreePerRadians, 1, 0, 0);
        // glRotatef(90, 1, 0, 0);
        propeller->transformAndDraw();
        glScalef(radius, radius, radius);
        drawAxis(radius);
        // glScalef(100, 100, 100);

        glPushMatrix();
        {
            glTranslatef(sCannonOffset.x, sCannonOffset.y, sCannonOffset.z);
            glScalef(2 / radius, 2 / radius, 2 / radius);
            drawCube();
        }
        glPopMatrix();

        if (sEnemyModel != NULL) {
            sEnemyModel->draw();
        } else {
    GLfloat mcolor[] = {0, 0.8f, 0, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mcolor);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mcolor);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mcolor);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20.0);
            drawCube();
        }
    }
    glPopMatrix();
}

void flying_enemy_t::init(wf_object_loader_t* loader) {
    // sEnemyModel = loader->loadRes("enemy");
}

void flying_enemy_t::setPosition(const point3f& p) {
    controller->setPosition(p);
}

vector3f flying_enemy_t::getVelocity() const {
    return controller->getVelocity();
}

const char* flying_enemy_t::getName() const {
    return "fly";
}

vector3f flying_enemy_t::getCannonExit() {
    vector3f v = sCannonOffset * radius;

    v.rotateX(controller->getHorizontalAngularVelocity());
    v.rotateY(controller->getVerticalAngle());
    v.rotateZ(horizontal);

    return v;
}

void flying_enemy_t::drawMapElement(circle_blueprint_t* blueprint) const {
    if (dead) return;
    glPushMatrix();
    {
        // Move the system to the enemy
        point3f p = controller->getPosition();
        glTranslatef(p.x, p.y, 0);
        blueprint->draw(true, radius);
    }
    glPopMatrix();
}
