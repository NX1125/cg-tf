
/* 
 * File:   bomb.cpp
 * Author: 2019202344
 * 
 * Created on 6 de Dezembro de 2019, 15:50
 */

#include "bomb.h"
#include "player.h"

float bomb_t::sGravityAcceleration = -10;

bomb_t::bomb_t(const point3f& offset, const vector3f& velocity) :
projectile_t(offset, velocity) {
}

void bomb_t::move(float time) {
    projectile_t::move(time);
}

void bomb_t::hit(obstacle_t* other) {
    printf("A bomb hit an obstacle\n");
    bomb_t::hit(other);
    // TODO Check if the obstacle is an enemy base
}

void bomb_t::setSGravityAcceleration(float acc) {
    bomb_t::sGravityAcceleration = acc;
}

void bomb_t::draw() const {
    // TODO Draw bomb model
}
