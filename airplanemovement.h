/* 
 * File:   airplane_movement_t.h
 * Author: gg
 *
 * Created on 4 de dezembro de 2019, 23:31
 */

#ifndef AIRPLANE_MOVEMENT_T_H
#define AIRPLANE_MOVEMENT_T_H

#include "shapes.h"
#include "behaviourupdate.h"

/**
 * The player movement is similar to an airplane (in the spec, it is called
 * airplane, so we use it interchangeably). During the movement, the airplane
 * has inertia, meaning that its velocity doesn't change, UNLESS there is an 
 * input from the user. There are two types of input to handle, input system
 * and magnitude changes.
 * 
 * The input system has a horizontal and vertical axis and each one has an angle
 * and an angular velocity. These velocities only changes the direction of the 
 * forward direction. The horizontal and vertical angles are similar to
 * longitude and latitude, respectively, in a geographic coordinate system.
 * 
 * The magnitude can be updated and it also updates the angular velocity. There
 * is a factor that is used to correct the angular velocity with the magnitude
 * change.
 */
class airplane_movement_t : public behaviour_update_t {
private:

    // cached keys. These are saved to support diagonal movement.

    float left = 0;
    float right = 0;
    float up = 0;
    float down = 0;

    /**
     * The longitude or horizontal angle, where the name comes from the 
     * geographic coordinate system.
     * 
     * It is an angle in radians.
     */
    float horizontal;

    /**
     * The latitude or vertical angle, where the name comes from the 
     * geographic coordinate system.
     * 
     * It is an angle in radians.
     */
    float vertical;

    /**
     * The angular velocity of the horizontal angle. Each time step, the current
     * horizontal angle is updated with this velocity.
     */
    float horizontalVelocity = 0;

    /**
     * The angular velocity of the vertical angle. Each time step, the current
     * vertical angle is updated with this velocity.
     */
    float verticalVelocity = 0;

    /**
     * The current velocity magnitude. This is used with the angles to create
     * a velocity vector. It allows the magnitude to be changed only when the 
     * user wants.
     */
    float magnitude;

    /**
     * The forward direction of the player multiplied by the current magnitude.
     */
    vector3f velocity;

    /**
     * The current position of the airplane. This is updated with the current
     * velocity in each time step.
     */
    point3f position;

    /**
     * A factor used to convert the horizontal movement with the magnitude of
     * the mouse to a value in radians per second.
     * 
     * This is used to compute the horizontal angular velocity.
     */
    float horizontalFactor = 0.005;

    /**
     * A factor used to convert the vertical movement with the magnitude of
     * the mouse to a value in radians per second.
     * 
     * This is used to compute the vertical angular velocity.
     */
    float verticalFactor = 0.005;

    /**
     * A factor used to increase or decrease the magnitude when the '+' or '-'
     * keys are typed.
     */
    float velocityAccelerationFactor = 0.1f;

public:

    airplane_movement_t();

    /**
     * A helper method that converts the user input into an axis for the angles.
     * The key is only used if it is 'a', 's', 'd', 'w', '+' or '-'.
     */
    void keyPress(unsigned char key);

    /**
     * A helper method that converts the user input into an axis for the angles.
     * The key is only used if it is 'a', 's', 'd', 'w', '+' or '-'.
     */
    void keyReleased(unsigned char key);

    vector3f getVelocity() const {
        return velocity;
    }

    void clipZ(float height, float radius);

private:

    void updateKey(unsigned char key, float intensity);

    void updateVelocity();

public:

    void update(int millis) {
        move(millis / 1000.0);
    }

    /**
     * Moves the current position by the velocity though a time interval.
     * It also rotates the current angles by their angular velocities.
     */
    void move(float dt);

    void setInputAxis(float dx, float dy);

    void setInitialConditions(const point3f& p, const vector3f& v);

    void setAngles(float horizontal, float vertical) {
        this->horizontal = horizontal;
        this->vertical = vertical;
    }

    void setPosition(const point3f& position) {
        this->position = position;
    }

    float getMagnitude() const {
        return magnitude;
    }

    void setMagnitude(float magnitude);

    float getHorizontalAngularVelocity() const {
        return -horizontalVelocity;
    }

    point3f getPosition() const {
        return position;
    }

    float getHorizontalAngle() const {
        return horizontal;
    }

    float getVerticalAngle() const {
        return -vertical;
    }

    float getVelocityMagnitude() const override {
        return magnitude;
    }

};

#endif /* AIRPLANE_MOVEMENT_T_H */

