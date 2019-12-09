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
#include "projectile.h"
#include "cannon.h"
#include "propeller.h"
#include "minimapitem.h"
#include "bomb.h"
#include "resetlistener.h"

enum Behaviour {
    ON_GROUND, TAKING_OFF, CONTROLLING, GAME_OVER
};

class player_death_listener_t {
public:
    // Bring some flowers, ok?
    virtual void onPlayerDeath() = 0;
};

class bomb_listener_t {
public:

    virtual void onBombThrow(bomb_t* b) = 0;
};

class player_t : public teleportable_t, public obstacle_t, public mini_map_item_t, public reset_listener_t {
private:
    static wf_object_t* sPlayerBodyModel;
    static float sPlayerBodyModelRadius;

    static vector3f sBombDoor;

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

    float horizontalAngularVelocityDrawing = 0;

    /**
     * The behaviour of the player through time. Initially, the player object
     * is waiting for the player to press 'u' to then start the takeoff.
     */
    Behaviour mBehaviour = Behaviour::ON_GROUND;

    takeoff_t* takeoff;
    airplane_movement_t* controller;

    cannon_t* cannon;

    propeller_t* propellerRight;
    propeller_t* propellerLeft;

    /**
     * The radius given in the settings.
     */
    float radius;

    float velocityFactor = 1.0f;

    bool dead = false;

    projectile_manager_t* manager = NULL;

    float bulletVelocityFactor = 1.0f;

    vector3f cockpitOffset;

    player_death_listener_t* death = NULL;

    bomb_listener_t* bombListener = NULL;

public:

    player_t(takeoff_t* takeoff, float radius);

    void cockpitView();

    void cannonView();

    void setManager(projectile_manager_t* manager);

    void setBulletVelocityFactor(float bulletVelocityFactor) {
        this->bulletVelocityFactor = bulletVelocityFactor;
    }

    void setDeathListener(player_death_listener_t* death) {
        this->death = death;
    }

    void setBombListener(bomb_listener_t* bombListener) {
        this->bombListener = bombListener;
    }

    /**
     * Moves the player to inside the arena if it hit the roof or the ground.
     */
    void clipZ(float height);

    void setVelocityFactor(float velocityFactor) {
        this->velocityFactor = velocityFactor;
    }

    void draw(bool cockpit, bool gun, bool body, bool aim);

    void keyPress(unsigned char key);

    void keyRelease(unsigned char key);

    void mousePress(int button);

    Behaviour getBehaviour() const {
        return mBehaviour;
    }

    point3f getPosition() const {
        return position;
    }

    float getRadius() const {
        return radius;
    }

    float getHorizontal() const {
        return horizontal;
    }

    float getVertical() const {
        return vertical;
    }

    vector3f getDirection(float v) const;

    vector3f getVelocity() const;

    void update(int millis);

    static void sInit(wf_object_loader_t& loader);

    bool canTeleport() const {
        return mBehaviour == Behaviour::CONTROLLING;
    }

    const char* getName() const override;

    void setPosition(const point3f& p) override;

    bool isDead() const {
        return dead;
    }

    /**
     * Throw a bomb.
     */
    void bomb();

    /**
     * Fire a bullet.
     */
    void fire();

    void kill();

    void won();

    bool canDie() const;

    /**
     * Both x and y are between 0 and 1.
     */
    void setCannonAxis(float x, float y);

    bool isAlive() const override {
        return !dead;
    }

    void drawMapElement(circle_blueprint_t* blueprint) const override;
    
    void reset() override {
        position = takeoff->getStart();
        mBehaviour = Behaviour::ON_GROUND;
        
        takeoff->reset();
        controller->reset();
        cannon->reset();
        
        propellerLeft->reset();
        propellerRight->reset();
        
        dead = false;
    }

};

#endif /* AIRPLANE_H */

