/* 
 * File:   thirdpersonfollower.cpp
 * Author: gg
 * 
 * Created on 4 de dezembro de 2019, 17:23
 */

#include "thirdpersonfollower.h"

third_person_follower_t::third_person_follower_t(const point3f& target, float normalDistance) :
target(target), camera(target), normalDistance(normalDistance) {
    camera.x += normalDistance;
}

void third_person_follower_t::lookAt() {
    gluLookAt(camera.x, camera.y, camera.z,
            target.x, target.y, target.z,
            0, 0, 1);
}

void third_person_follower_t::setMousePressingPosition(int x, int y) {
    previousMousePositionX = x;
    previousMousePositionY = y;
}

/**
 * Called when the mouse were moved. The camera rotates around the target
 * with horizontal and vertical movememnts.
 */
void third_person_follower_t::mouseDragged(int x, int y) {
    int dx = x - previousMousePositionX;
    int dy = y - previousMousePositionY;

    move(-dx, dy);

    previousMousePositionX = x;
    previousMousePositionY = y;
}

float clampAngle(float angle) {
    float t = fmod(angle, M_PI * 2);
    if (angle > 0) {
        return t;
    } else {
        return M_PI * 2 + t;
    }
}

void third_person_follower_t::move(float dx, float dy) {
    vector3f v = camera - target;

    float distance = v.normalize();

    // convert the target-camera vector to angles.
    float horizontal = atan2(v.y, v.x) + dx * horizontalFactor;
    float vertical = asinf((camera.z - target.z) / distance);

    vertical += dy * verticalFactor;

    setAngle(horizontal, vertical, distance);
}

void third_person_follower_t::setAngle(float horizontal, float vertical) {
    setAngle(horizontal, vertical, (camera - target).length());
}

// FIXME Why does the vertical angle doesn't work normally and need to be inverted?

void third_person_follower_t::setAngle(float horizontal, float vertical,
        float distance) {
    // do this for some reason I don't know why
    // vertical = -vertical;

    horizontal = clampAngle(horizontal);

    // clamp the vertical angle to -60° and 60°
    if (vertical < -maxVerticalAngle) {
        vertical = -maxVerticalAngle;
    } else if (vertical > maxVerticalAngle) {
        vertical = maxVerticalAngle;
    }

    // rotate <1,0,0> by vertical around the y axis
    float r = cosf(vertical);
    float z = sinf(vertical);

    // rotate <x, 0, z> by horizontal around the z axis
    // The z axis continues to be the same.

    // printf("h = %f, v = %f\n", horizontal, vertical);

    // g is from Ground
    float x = cosf(horizontal) * r;
    float y = sinf(horizontal) * r;

    // <x, y, z> is the vector of the camera
    camera = target + vector3f(x, y, z) * distance;
    //     printf("camera = (%f, %f, %f)\n", camera.x, camera.y, camera.z);
}

void third_person_follower_t::follow(float dt) {
    vector3f v = camera - target;
    float length = v.normalize();

    camera = target + v * normalDistance;
}