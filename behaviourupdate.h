/* 
 * File:   behaviourupdate.h
 * Author: gg
 *
 * Created on 5 de dezembro de 2019, 23:44
 */

#ifndef BEHAVIOURUPDATE_H
#define BEHAVIOURUPDATE_H

/**
 * An interface implemented by behaviours of the player object that can update
 * the position and angles of it.
 */
class behaviour_update_t {
public:

    virtual void update(int millis) = 0;

    virtual point3f getPosition() const = 0;

    virtual float getHorizontalAngle() const = 0;

    virtual float getVerticalAngle() const = 0;

    virtual float getHorizontalAngularVelocity() const = 0;
};

#endif /* BEHAVIOURUPDATE_H */

