
/* 
 * File:   propeller.cpp
 * Author: 2019202344
 * 
 * Created on 6 de Dezembro de 2019, 20:15
 */

#include "propeller.h"
#include "cube.h"

propeller_t::propeller_t(vector3f offset) :
offset(offset) {
}

void propeller_t::draw() {
    glPushMatrix();
    {
        glTranslatef(offset.x, offset.y, offset.z);
        glRotatef(angle, 0.0f, 0.0f, 1.0f);

        // TODO Draw model
        glScalef(10, 1, 1);
        cube_t::drawBox();
    }
    glPopMatrix();
}

void propeller_t::setMagnitude(float v) {
    angularVelocity = v * factor;
}

void propeller_t::setScaleFactor(float scale) {
    this->factor = scale * 360.0f;
}

void propeller_t::update(int millis) {
    angle += angularVelocity * millis / 1000.0f;
    float before = angle;
    angle = clampAngle(angle);
    
    int i = 0;
}
