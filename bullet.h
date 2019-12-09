/* 
 * File:   bullet.h
 * Author: gg
 *
 * Created on 7 de dezembro de 2019, 01:09
 */

#ifndef BULLET_H
#define BULLET_H

#include "projectile.h"
#include "wfobj.h"

class bullet_t : public projectile_t {
private:
    
    static wf_object_t* sModel;

    bool enemy;

    float radius = 10.0f;
    
    float horizontal = 0;
    float vertical = 0;
    
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
    
    static void init(wf_object_loader_t& loader);
};

#endif /* BULLET_H */

