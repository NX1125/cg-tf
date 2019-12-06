/* 
 * File:   airplane_movement_t.cpp
 * Author: gg
 * 
 * Created on 4 de dezembro de 2019, 23:31
 */

#include "airplanemovement.h"

#include <cmath>
#include <cstdio>

void airplane_movement_t::keyPress(unsigned char key) {
    switch (key) {
        case '+':
            velocity *= 1 + velocityAccelerationFactor;
            break;
        case '-':
            velocity *= 1 - velocityAccelerationFactor;
            break;
        default:
            updateKey(key, 1);
            break;
    }
}

void airplane_movement_t::keyReleased(unsigned char key) {
    updateKey(key, 0);
}

void airplane_movement_t::updateKey(unsigned char key, float intensity) {
    switch (key) {
        case 'a':
            left = intensity;
            break;
        case 'd':
            right = intensity;
            break;
        case 's':
            down = intensity;
            break;
        case 'w':
            up = intensity;
            break;
        default:
            return;
    }

    setInputAxis(right - left, up - down);
}

void airplane_movement_t::move(float dt) {
    horizontal += horizontalVelocity * dt;
    vertical += verticalVelocity * dt;
    updateVelocity();
    position += velocity * dt;
}

void airplane_movement_t::updateVelocity() {
    float r, z;

    // compute the sine and cosine
    sincosf(vertical, &z, &r);

    float x, y;

    sincosf(horizontal, &y, &x);

    x *= r;
    y *= r;

    // this velocity is normalized, its length is 1.
    velocity.x = x;
    velocity.y = y;
    velocity.z = z;

    velocity *= magnitude;
}

void airplane_movement_t::setInputAxis(float dx, float dy) {
    horizontalVelocity = dx * magnitude * horizontalFactor;
    verticalVelocity = dy * magnitude * verticalFactor;
}

void airplane_movement_t::setMagnitude(float magnitude) {
    velocity *= magnitude / this->magnitude;
    this->magnitude = magnitude;
}

void airplane_movement_t::setInitialConditions(const point3f& p, const vector3f& v) {
    position = p;
    velocity = v;

    magnitude = velocity.length();

    // The angle of the vector <x, y> is the horizontal angle.
    horizontal = atan2f(velocity.y, velocity.x);
    // This projects the vector to the xy plane and uses the z component 
    // as opposite cathetus while the length of xy is the adjacent cathetus.
    vertical = asinf(velocity.z / magnitude);
}
