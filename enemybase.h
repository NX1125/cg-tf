
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
#include "minimapitem.h"

class base_death_t {
public:
    virtual void onBaseDeath() = 0;
};

class enemy_base_t : public obstacle_t, public mini_map_item_t {
private:

    static wf_object_t* sEnemyModel;

    point3f position;

    float radius;

    bool dead = false;

    base_death_t* deathListener = NULL;

public:

    enemy_base_t(point3f position, float radius);

    void reset() {
        dead = false;
    }
    
    void transformAndDraw();

    static void init(wf_object_loader_t& loader);

    void kill() {
        if (dead) return;

        dead = true;

        if (deathListener != NULL) {
            deathListener->onBaseDeath();
        }
    }

    void setDeathListener(base_death_t* l) {
        this->deathListener = l;
    }

    point3f getPosition() const override {
        return position;
    }

    float getRadius() const override {
        return radius;
    }

    bool isAlive() const override {
        return !dead;
    }

    void drawMapElement(circle_blueprint_t* blueprint) const override;
};

#endif /* ENEMYBASE_H */

