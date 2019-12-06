/* 
 * File:   teleportable.h
 * Author: 2019202344
 *
 * Created on 6 de Dezembro de 2019, 13:01
 */

#ifndef TELEPORTABLE_H
#define TELEPORTABLE_H

#include "shapes.h"

/**
 * An object that can teleport when it hits the arena walls. The arena is
 * centered at the origin.
 */
class teleportable_t {
public:

    /**
     * 
     */
    void teleport(float bounds);

    virtual vector3f getVelocity() const = 0;

    virtual point3f getPosition() const = 0;

    virtual void setPosition(const point3f& p) = 0;
    
    virtual float getRadius() const = 0;
    
    virtual const char* getName() const = 0;
};

#endif /* TELEPORTABLE_H */

