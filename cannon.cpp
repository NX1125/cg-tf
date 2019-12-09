
/* 
 * File:   cannon.cpp
 * Author: 2019202344
 * 
 * Created on 6 de Dezembro de 2019, 19:54
 */

#include "cannon.h"
#include "cube.h"

wf_object_t* cannon_t::sModel = NULL;

cannon_t::cannon_t(const vector3f& offset) :
offset(offset) {
}

void cannon_t::draw() {
    glPushMatrix();
    {
        glTranslatef(offset.x, offset.y, offset.z);

        glRotatef(horizontal, 0, 0, 1);
        glRotatef(-vertical, 0, 1, 0);

        if (sModel == NULL) {
            glScaled(getLength() / 2, 1, 1);
            glTranslatef(1, 0, 0);
            drawCube();
        } else {
            sModel->draw();
        }
    }
    glPopMatrix();
}

void cannon_t::setInputAxis(float x, float y) {
    const float maxAngle = 30;
    horizontal = -(x * 2 * maxAngle - maxAngle);
    vertical = -(y * 2 * maxAngle - maxAngle);
}

vector3f cannon_t::getDirection() const {
    return getDirection(0, 0);
}

vector3f cannon_t::getDirection(float h, float v) const {
    float r;
    float z;

    const float dr = M_PI / 180.0f;

    sincosf(vertical * dr + v, &z, &r);

    float x, y;

    sincosf(horizontal * dr + h, &y, &x);

    return vector3f(r * x, r * y, z);
}
