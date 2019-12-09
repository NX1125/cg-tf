
/* 
 * File:   bomb.cpp
 * Author: 2019202344
 * 
 * Created on 6 de Dezembro de 2019, 15:50
 */

#include "bomb.h"
#include "player.h"
#include "cube.h"
#include "enemybase.h"

float bomb_t::sGravityAcceleration = -10;

wf_object_t* bomb_t::sBombModel = NULL;

void bomb_t::init(wf_object_loader_t& loader) {
    sBombModel = loader.loadRes("presente");
}

bomb_t::bomb_t(const point3f& offset, const vector3f& velocity, float h) :
projectile_t(offset, velocity), horizontal(h) {
}

void bomb_t::move(float time) {
    projectile_t::move(time);
    getVelocity().z += time * sGravityAcceleration;
}

void bomb_t::hit(obstacle_t* other) {
    printf("A bomb hit an obstacle\n");
    projectile_t::hit(other);
    if (enemy_base_t * base = dynamic_cast<enemy_base_t*> (other)) {
        printf("The bomb fell onto a base\n");
        base->kill();
    }
}

void bomb_t::setGravityAcceleration(float acc) {
    bomb_t::sGravityAcceleration = acc;
}

void bomb_t::draw() const {
    // TODO Draw bomb model
    glRotatef(horizontal, 0, 0, 1);
    glRotatef(90, 1, 0, 0);
    if (sBombModel == NULL) {
        drawCube();
    } else {
        sBombModel->draw();
    }
}

