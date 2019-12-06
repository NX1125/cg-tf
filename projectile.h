
/* 
 * File:   projectile_t.h
 * Author: 2019202344
 *
 * Created on 6 de Dezembro de 2019, 15:21
 */

#ifndef PROJECTILE_T_H
#define PROJECTILE_T_H

#include <GL/glu.h>

#include <vector>

#include "shapes.h"

/**
 * An abstract class that uses a projectile movement to move the object.
 */
class projectile_t {
private:

    static std::vector<projectile_t*> sProjectileQueue;
    
    /**
     * The initial point that the projectile started.
     */
    point3f offset;

    /**
     * The current position of the projectile.
     */
    point3f position;

    /**
     * The current velocity of the projectile.
     */
    vector3f velocity;

    float accumulatedTime = 0;

    bool dead = false;

public:

    projectile_t(const point3f& offset, const vector3f& velocity);

    void update(int millis);

    virtual void move(float time);

    bool isDead() const {
        return dead;
    }
    
    void kill() {
        dead = true;
    }

    void transformAndDraw() const;
    
    static void addProjectile(projectile_t* p);

protected:
    virtual void draw() const = 0;
};

#endif /* PROJECTILE_T_H */

