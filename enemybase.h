
/* 
 * File:   enemybase.h
 * Author: 2019202344
 *
 * Created on 6 de Dezembro de 2019, 18:16
 */

#ifndef ENEMYBASE_H
#define ENEMYBASE_H

#include "shapes.h"
#include "airplanemovement.h"
#include "wfobj.h"
#include "projectile.h"

class enemy_base_t : public obstacle_t {
private:

    static wf_object_t* sEnemyModel;

    point3f position;

    float radius;

    bool dead = false;

public:

    enemy_base_t(point3f position, float radius);

    void transformAndDraw();

    static void init(wf_object_loader_t* loader);

    void kill() {
        dead = true;
    }

    point3f getPosition() const override {
        return position;
    }

    float getRadius() const override {
        return radius;
    }

};

#endif /* ENEMYBASE_H */

