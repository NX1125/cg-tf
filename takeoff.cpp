/* 
 * File:   takeoff_t.cpp
 * Author: gg
 * 
 * Created on 4 de dezembro de 2019, 23:27
 */

#include "takeoff.h"

#include <math.h>

takeoff_t::takeoff_t(const point3f& start, const point3f& end,
        float height, unsigned int timeToComplete) :
start(start), end(end), height(height), timeToComplete(timeToComplete) {
    normal = end - start;
    length = normal.normalize();

    float dt = timeToComplete / 1000.0f;
    acceleration = 2 * length / (dt * dt);
}

void takeoff_t::reset() {
    position = start;
    direction = normal;
    currentTime = 0;
    completed = false;
}

void takeoff_t::run(int time) {
    // Check if we didn't reached the end of the airstrip
    if (completed || currentTime > timeToComplete) {
        completed = true;
        return;
    }

    currentTime += timeToComplete;

    float dt = time / 1000.0f;

    float distance = acceleration * dt * dt / 2;

    float k = distance / length;

    // distance is after the middle of the airstrip
    // d > l / 2
    // 2d > l
    if (distance * 2 > length) {
        setTakeoff(k);
    } else {
        setGround(k);
    }
}

void takeoff_t::setGround(float k) {
    position = start + (end - start) * k ;
    direction = normal;
}

void takeoff_t::setTakeoff(float k) {
    setGround(k);
    float k2 = (k - 0.5f) / 0.5f;
    position.z = getTakeoffHeight(k2);
    direction.z = getTakeoffTangentZ(k2);
}

float takeoff_t::getTakeoffHeight(float x) const {
    return height * sinf(x * M_PI / 2);
}

float takeoff_t::getTakeoffTangentZ(float k) const {
    // derivative of getTakeoffHeight
    return height * M_PI / 2 * cosf(k * M_PI / 2);
}


