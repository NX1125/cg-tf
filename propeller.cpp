
/* 
 * File:   propeller.cpp
 * Author: 2019202344
 * 
 * Created on 6 de Dezembro de 2019, 20:15
 */

#include <cstdio>

#include "propeller.h"
#include "cube.h"

wf_object_t* propeller_t::sModel = NULL;

propeller_t::propeller_t(vector3f offset) :
offset(offset) {
}

void propeller_t::init(wf_object_loader_t& loader) {
    sModel = loader.loadRes("helice");
}

void propeller_t::transformAndDraw(float s) const {
    glPushMatrix();
    {
        glTranslatef(offset.x, offset.y, offset.z);
        glRotatef(90, 0.0f, 0.0f, 1);
        glRotatef(angle * 180 / M_PI, 0.0f, 1.0f, 0.0f);
        glScalef(s, s, s);

        // printf("angle = %f\n", angle);

        // drawAxis(1);
        
        if (sModel == NULL) {
            glScalef(10, 1, 1);
            drawCube();
        } else {
            sModel->draw();
        }
    }
    glPopMatrix();
}

void propeller_t::setMagnitude(float v) {
    angularVelocity = v * factor;
}

void propeller_t::setScaleFactor(float scale) {
    this->factor = scale * 2 * M_PI;
}

void propeller_t::update(int millis) {
    angle += angularVelocity * millis / 1000.0f;
    angle = clampAngle(angle);
}
