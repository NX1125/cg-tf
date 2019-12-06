
/* 
 * File:   flyingenemy.h
 * Author: 2019202344
 *
 * Created on 6 de Dezembro de 2019, 18:22
 */

#ifndef FLYINGENEMY_H
#define FLYINGENEMY_H

#include <random>

#include "wfobj.h"
#include "airplanemovement.h"

class flying_enemy_t {
private:
    
    static std::default_random_engine sRandomMovement;

    static wf_object_t* sEnemyModel;

    float radius;

    airplane_movement_t* controller;

public:

    flying_enemy_t(point3f position, float radius);

    void update(int millis);
    
    void transformAndDraw();

    static void init(wf_object_loader_t* loader);
};

#endif /* FLYINGENEMY_H */

