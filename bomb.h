
/* 
 * File:   bomb.h
 * Author: 2019202344
 *
 * Created on 6 de Dezembro de 2019, 15:50
 */

#ifndef BOMB_H
#define BOMB_H

#include "projectile.h"


class bomb_t : public projectile_t {
private:
    
    static float sGravityAcceleration ;
public:

    bomb_t(const point3f& offset, const vector3f& velocity);
    
    void move(float time) override;
    
    void hit(obstacle_t* other) override;

    static void setSGravityAcceleration(float acc);
    
    void draw() const override;
    
    float getRadius() const override {
        // TODO Measure bomb radius
        return 0;
    }
};

#endif /* BOMB_H */

