
/* 
 * File:   projectile_t.cpp
 * Author: 2019202344
 * 
 * Created on 6 de Dezembro de 2019, 15:21
 */

#include "projectile.h"

std::vector<projectile_t*> projectile_t::sProjectileQueue;

projectile_t::projectile_t(const point3f& offset, const vector3f& velocity) :
offset(offset), position(offset), velocity(velocity) {
}

void projectile_t::update(int millis) {
    accumulatedTime += millis / 1000.0f;
    move(accumulatedTime);
}

void projectile_t::move(float time) {
    position = offset + velocity * time;
}

void projectile_t::transformAndDraw() const {
    glPushMatrix();
    {
        point3f v = position + velocity;
        gluLookAt(position.x, position.y, position.z,
                v.x, v.y, v.z,
                0.0f, 0.0f, 1.0f);

        draw();
    }
    glPopMatrix();
}

void projectile_t::addProjectile(projectile_t* p) {
    sProjectileQueue.push_back(p);
}
