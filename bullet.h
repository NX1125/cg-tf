/* 
 * File:   bullet.h
 * Author: gg
 *
 * Created on 7 de dezembro de 2019, 01:09
 */

#ifndef BULLET_H
#define BULLET_H

#include "projectile.h"

class bullet_t : public projectile_t {
private:

    bool enemy;

    float radius = 1.0f;
    
public:

    bullet_t(const point3f& offset, const vector3f& velocity, bool enemy);
    
    void setRadius(float radius) {
        this->radius = radius;
    }

    void hit(obstacle_t* other) override;

    void draw() const override;
    
    float getRadius() const override {
        return radius;
    }
};

#endif /* BULLET_H */

