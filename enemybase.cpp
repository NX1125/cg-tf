
/* 
 * File:   enemybase.cpp
 * Author: 2019202344
 * 
 * Created on 6 de Dezembro de 2019, 18:16
 */

#include "enemybase.h"

#include <GL/glut.h>
#include "cube.h"

wf_object_t* enemy_base_t::sEnemyModel = NULL;

enemy_base_t::enemy_base_t(point3f position, float radius) :
position(position), radius(radius) {
}

void enemy_base_t::transformAndDraw() {
    if (dead) {
        return;
    }
    glPushMatrix();
    {
        glTranslatef(position.x, position.y, position.z);
        drawAxis(radius);
        // glRotatef(90, 1, 0, 0);
        glScalef(radius, radius, radius);
        // glScalef(100, 100, 100);

        glColor3f(1, 1, 1);
        if (sEnemyModel != NULL) {
            glRotatef(90, 1, 0, 0);
            sEnemyModel->draw();
        } else {
            // draw all of the ground enemies in orange (#FFA500)
            // glColor3f(0xFF / 255.0, 0xA5 / 255.0, 0x00 / 255.0);
            drawCube();
        }
    }
    glPopMatrix();
}

void enemy_base_t::init(wf_object_loader_t& loader) {
    sEnemyModel = loader.loadRes("casa");
}

void enemy_base_t::drawMapElement(circle_blueprint_t* blueprint) const {
    if (dead) return;

    glPushMatrix();
    {
        glTranslatef(position.x, position.y, 0);
        blueprint->draw(true, radius);
    }
    glPopMatrix();
}

