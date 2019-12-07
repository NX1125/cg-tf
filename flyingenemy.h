
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
#include "projectile.h"
#include "teleportable.h"

class flying_enemy_t : public obstacle_t, public teleportable_t {
private:

    static std::default_random_engine sRandomMovement;

    static wf_object_t* sEnemyModel;

    float radius;

    airplane_movement_t* controller = NULL;

    bool dead = false;

    float initialVelocity;
    
    time_t accumulatedTime = 0;
    
    int behaviour = 0;

    float horizontal;
    
public:

    flying_enemy_t(point3f position, float radius);

    void setInitialVelocity(float initialVelocity);

    void update(int millis);

    void clipZ(float z);

    void transformAndDraw();

    static void init(wf_object_loader_t* loader);

    point3f getPosition() const override;

    float getRadius() const override;

    void kill();

    bool isDead() const {
        return dead;
    }

    const char* getName() const override;

    vector3f getVelocity() const override;

    void setPosition(const point3f& p) override;

};

#endif /* FLYINGENEMY_H */

