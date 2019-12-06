
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
        glRotatef(vertical, 0, 1, 0);

        if (sModel == NULL) {
            glScaled(10, 1, 1);
            glTranslatef(1,0,0);
            cube_t::drawBox();
        } else {
            sModel->draw();
        }
    }
    glPopMatrix();
}

void cannon_t::setInputAxis(float x, float y) {
    horizontal = -(x * 90 - 45);
    vertical = y * 90 - 45;
}
