
/* 
 * File:   projectile_t.cpp
 * Author: 2019202344
 * 
 * Created on 6 de Dezembro de 2019, 15:21
 */

#include <cstdio>

#include "projectile.h"

std::vector<projectile_t*> projectile_t::sProjectileQueue;

projectile_t::projectile_t(const point3f& offset, const vector3f& velocity) :
offset(offset), position(offset), velocity(velocity) {
}

point3f projectile_t::getPosition() const {
    return position;
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
        glTranslatef(position.x, position.y, position.z);
//        gluLookAt(position.x, position.y, position.z,
//                v.x, v.y, v.z,
//                0.0f, 0.0f, 1.0f);

        draw();
    }
    glPopMatrix();
}

void projectile_t::hit(obstacle_t* other) {
    kill();
}

void projectile_t::addProjectile(projectile_t* p) {
    sProjectileQueue.push_back(p);
}

void projectile_manager_t::addObstacles(obstacle_t* obs, bool animated) {
    if (animated) {
        animatedObstacles.push_back(obs);
    } else {
        inanimateObstacles.push_back(obs);
    }
}

void projectile_manager_t::addProjectile(projectile_t* p) {
    projectiles.push_back(p);
}

void projectile_manager_t::update(int millis) {
    for (projectile_t* p : projectiles) {
        p->update(millis);
    }

    for (obstacle_t* obs : animatedObstacles) {
        if (obs->isAlive()) {
            hit(obs);
        }
    }
    removeDeadProjectiles();
    for (obstacle_t* obs : inanimateObstacles) {
        hit(obs);
    }
    removeDeadProjectiles();
}

void projectile_manager_t::hit(obstacle_t* o) {
    for (int i = 0; i < projectiles.size(); i++) {
        if (projectiles[i]->overlaps(o)) {
            projectiles[i]->hit(o);
            break;
        }
    }
}

void projectile_manager_t::removeOutsideOfArena(float radius, float height) {
    for (projectile_t* p : projectiles) {
        p->clip(radius, height);
    }
    removeDeadProjectiles();
}

void projectile_manager_t::removeDeadProjectiles() {
    int n = projectiles.size();
    for (int i = 0; i < projectiles.size(); i++) {
        if (projectiles[i]->isDead()) {
            projectile_t* p = projectiles[i];
            projectiles.erase(projectiles.begin() + i);
            delete p;
            i--;
        }
    }
    n -= projectiles.size();
    if (n != 0) {
        printf("There were %d projectiles removed\n", n);
    }
}

void projectile_manager_t::draw() {
    for (projectile_t* p : projectiles) {
        p->transformAndDraw();
    }
}

