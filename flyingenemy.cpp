
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

flying_enemy_t::flying_enemy_t(point3f position, float radius) :
radius(radius) {
    controller = new airplane_movement_t( );
    controller->setPosition(position);
}

void flying_enemy_t::update(int millis) {
    controller->update(millis);
}

void flying_enemy_t::transformAndDraw() {
    glPushMatrix();
    {
        point3f p = controller->getPosition();
        glTranslatef(p.x, p.y, p.z);
        drawAxis(radius);
        // considering that the front of the airplane is at +x and the back is at -x
        const float degreePerRadians = 180 / M_PI;
        glRotatef(controller->getHorizontalAngle() * degreePerRadians, 0, 0, 1);
        glRotatef(controller->getVerticalAngle() * degreePerRadians, 0, 1, 0);
        glRotatef(controller->getHorizontalAngularVelocity() * degreePerRadians, 1, 0, 0);
        glRotatef(90, 1, 0, 0);
        glScalef(radius, radius, radius);
        drawAxis(radius);
        // glScalef(100, 100, 100);

        if (sEnemyModel != NULL) {
            sEnemyModel->draw();
        } else {
            cube_t::drawBox();
        }
    }
    glPopMatrix();
}

void flying_enemy_t::init(wf_object_loader_t* loader) {
    // sEnemyModel = loader->loadRes("enemy");
}

