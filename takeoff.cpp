/* 
 * File:   takeoff_t.cpp
 * Author: gg
 * 
 * Created on 4 de dezembro de 2019, 23:27
 */

#include "takeoff.h"

#include <cmath>
#include <cstdio>

takeoff_t::takeoff_t(const point3f& start, const point3f& end,
        float height, unsigned int timeToComplete) :
start(start), end(end), height(height), timeToComplete(timeToComplete) {
    normal = end - start;
    length = normal.normalize();

    float dt = timeToComplete / 1000.0f;
    acceleration = 2 * length / (dt * dt);

    printf("Acceleration on xy of the airplane is %f\n", acceleration);

    // The horizontal angle is the angle of the airstrip on the xy plane, 
    // and since the airstrip doesn't move, the angle is always the same.
    // So we just compute it once and return it.
    horizontalAngle = atan2f(normal.y, normal.x);
}

void takeoff_t::reset() {
    position = start;
    secondHalf = false;
    currentTime = 0;
    completed = false;
    velocity = 0;
}

void takeoff_t::update(int time) {
    // Check if we didn't reached the end of the airstrip
    if (completed || currentTime > timeToComplete) {
        printf("Takeoff completed in %ld ms\n", currentTime);

        position = end;
        position.z = height;

        verticalAngle = 0;

        completed = true;
        return;
    }

    currentTime += time;

    float dt = currentTime / 1000.0f;

    float distance = acceleration * dt * dt / 2;

    float k = distance / length;

    velocity = acceleration * dt;
    
    // distance is after the middle of the airstrip
    // d > l / 2
    // 2d > l
    if (distance * 2 > length) {
        if (!secondHalf) {
            secondHalf = true;
            printf("The second half of the airstrip were reached in %ld ms\n", currentTime);
        }
        setTakeoff(k, distance);
    } else {
        setGround(k);
    }
}

float takeoff_t::getFinalVelocity() const {
    // s(t) = a / 2 * t^2
    // s'(t) = a / 2 * t * 2
    // s'(t) = at
    // v(t) = at
    return (timeToComplete / 1000.0f) * acceleration;
}

void takeoff_t::setGround(float k) {
    position = start + (end - start) * k;

    verticalAngle = 0;
}

void takeoff_t::setTakeoff(float k, float distance) {
    setGround(k);
    float k2 = (k - 0.5f) / 0.5f;
    position.z += getTakeoffHeight(k2);
    float r = distance - length * 0.5f;

    // The vertical angle is a bit complicated, since it uses the tangent of a
    // function to be the slope. We have to compute the arctan of it.
    verticalAngle = -atanf(height * getTakeoffTangent(k2) / (length / 2));

    //     printf("z = %f\n", direction.z);
    // printf("v = %f\n", verticalAngle);
    // printf("k2 = %f\n", k2);
}

float takeoff_t::getTakeoffHeight(float x) const {
    return height * getTakeoffFactor(x);
}

float takeoff_t::getTakeoffFactor(float x) {
    // The idea here is to scale the sine wave of [-pi, pi] to [0,1].
    // f(t) = sin(t), f(0) = 0, f(1) = sin(1)
    // f(t) = sin(t - pi / 2), f(0) = sin(-pi/2)=-1, f(1) = sin(1 - pi / 2)
    // f(t) = sin(pi * t - pi / 2), f(0) = sin(-pi/2)=-1, f(1) = sin(pi - pi / 2) = 1
    // f(t) = sin(pi * t - pi / 2) + 1, f(0) = sin(-pi/2) + 1=0, f(1) = 1 + 1 = 2
    // f(t) = (sin(pi * t - pi / 2) + 1) / 2, f(0) = 0 / 2 = 0, f(1) = 2 / 2 = 1
    // f(t) = (sin(pi * (t - 1 / 2)) + 1) / 2, f(0) = 0 / 2 = 0, f(1) = 2 / 2 = 1
    return (sinf(M_PI * (x - 0.5f)) + 1) * 0.5f;
}

float takeoff_t::getTakeoffTangentZ(float k) const {
    // derivative of getTakeoffHeight
    // f(t) = (sin(pi * (t - 1 / 2)) + 1) / 2
    // f'(t) = (pi * cos(pi * (t - 1 / 2)) ) / 2
    return height * getTakeoffTangent(k);
}

float takeoff_t::getTakeoffTangent(float k) {
    // derivative of getTakeoffHeight
    // f(t) = (sin(pi * (t - 1 / 2)) + 1) / 2
    // f'(t) = (pi * cos(pi * (t - 1 / 2)) ) / 2
    // f'(t) = (pi * cos(pi * (t - 1 / 2)) ) / 2
    return M_PI / 2 * cosf(M_PI * (k - 0.5f));
}

