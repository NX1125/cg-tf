/* 
 * File:   bullet.cpp
 * Author: gg
 * 
 * Created on 7 de dezembro de 2019, 01:09
 */

#include "bullet.h"
#include "cube.h"
#include "flyingenemy.h"
#include "player.h"

wf_object_t* bullet_t::sModel = NULL;

bullet_t::bullet_t(const point3f& offset, const vector3f& velocity, bool enemy) :
projectile_t(offset, velocity), enemy(enemy) {
}

void bullet_t::draw() const {
    // TODO Draw bullet model
    //    glRotatef(horizontal, 0, 0, 1);
    //    glRotatef(vertical, 1, 0, 0);
    if (sModel != NULL) {
        sModel->draw();
    } else {
        drawCube();
    }
}

void bullet_t::hit(obstacle_t* other) {
    printf("A bullet hit an obstacle\n");
    projectile_t::hit(other);
    if (enemy) {
        if (player_t * p = dynamic_cast<player_t*> (other)) {
            printf("The bullet hit the player\n");
            p->kill();
        }
    } else {
        if (flying_enemy_t * base = dynamic_cast<flying_enemy_t*> (other)) {
            printf("The bullet hit an enemy\n");
            base->kill();
        }
    }
}

void bullet_t::init(wf_object_loader_t& loader) {
    sModel = loader.loadRes("duende");
}

