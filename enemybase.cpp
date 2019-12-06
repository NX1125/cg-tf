
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
    glPushMatrix();
    {
        glTranslatef(position.x, position.y, position.z);
        drawAxis(radius);
        // glRotatef(90, 1, 0, 0);
        glScalef(radius, radius, radius);
        // glScalef(100, 100, 100);

        if (sEnemyModel != NULL) {
            sEnemyModel->draw();
        } else {
            cube_t::drawBox();
        }
    }
    glPopMatrix();
}

void enemy_base_t::init(wf_object_loader_t* loader) {
    // sEnemyModel = loader->loadRes("enemy");
}

