/* 
 * File:   airplane.h
 * Author: gg
 *
 * Created on 5 de dezembro de 2019, 19:41
 */

#ifndef AIRPLANE_H
#define AIRPLANE_H

#include "wfobj.h"
#include "takeoff.h"
#include "airplanemovement.h"
#include "teleportable.h"

enum Behaviour {
    ON_GROUND, TAKING_OFF, CONTROLLING, GAME_OVER
};

class player_t : public teleportable_t{
private:
    static wf_object_t* sPlayerBodyModel;

    /**
     * The location that the player will be drawn. The position can be updated
     * by overriding it.
     */
    point3f position;

    /**
     * The horizontal angle of the model to be rendered. This is done in the xy
     * plane. It is similar to longitude.
     */
    float horizontal = 0;

    /**
     * The vertical angle of the model to be rendered. It is similar to
     * latitude.
     */
    float vertical = 0;

    /**
     * The velocity of the horizontal angle. The airplane rotates around the
     * axis from the back to the front with this angle to have an effect of
     * movement.
     */
    float horizontalAngularVelocity = 0;

    /**
     * The behaviour of the player through time. Initially, the player object
     * is waiting for the player to press 'u' to then start the takeoff.
     */
    Behaviour mBehaviour = Behaviour::ON_GROUND;

    takeoff_t* takeoff;
    airplane_movement_t* controller;

    /**
     * The radius given in the settings.
     */
    float radius;

    float velocityFactor = 1.0f;

public:

    player_t(takeoff_t* takeoff, float radius);

    void setVelocityFactor(float velocityFactor) {
        this->velocityFactor = velocityFactor;
    }

    void draw();

    void keyPress(unsigned char key);

    void keyRelease(unsigned char key);

    Behaviour getBehaviour() const {
        return mBehaviour;
    }

    point3f getPosition() const {
        return position;
    }

    float getRadius() const {
        return radius;
    }
    
    vector3f getVelocity() const;

    void update(int millis);

    static void sInit(wf_object_loader_t& loader);

    bool canTeleport() const {
        return mBehaviour == Behaviour::CONTROLLING;
    }
    
    const char* getName() const override;
    
    void setPosition(const point3f& p) override;
};

#endif /* AIRPLANE_H */
