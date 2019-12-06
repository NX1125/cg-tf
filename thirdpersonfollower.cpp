/* 
 * File:   thirdpersonfollower.cpp
 * Author: gg
 * 
 * Created on 4 de dezembro de 2019, 17:23
 */

#include "thirdpersonfollower.h"

third_person_follower_t::third_person_follower_t(const point3f& target, float normalDistance) :
target(target), camera(target), normalDistance(normalDistance), offset(0, 0, 1) {
    camera.x += normalDistance;
}

void third_person_follower_t::lookAt() {
    point3f p = getFocus();
    gluLookAt(camera.x, camera.y, camera.z,
            p.x, p.y, p.z,
            0, 0, 1);
}

void third_person_follower_t::lookAtDebug() {
    gluLookAt(target.x + normalDistance, target.y, target.z + normalDistance * 0.1,
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

void third_person_follower_t::move(float dx, float dy) {
    vector3f v = camera - target;

    float distance = v.normalize();

    // convert the target-camera vector to angles.
    float horizontal = atan2f(v.y, v.x) + dx * horizontalFactor;
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

    // convert the target-camera vector to angles.
    float vertical = asinf((camera.z - target.z) / v.normalize());

    bool invalidated = true;

    // clamp the vertical angle to -60° and 60°
    if (vertical < -maxVerticalAngle) {
        vertical = -maxVerticalAngle;
    } else if (vertical > maxVerticalAngle) {
        vertical = maxVerticalAngle;
    } else {
        invalidated = false;
    }

    if (invalidated) {
        float horizontal = atan2f(v.y, v.x);

        // rotate <1,0,0> by vertical around the y axis
        float r = cosf(vertical);
        v.z = sinf(vertical);

        // rotate <x, 0, z> by horizontal around the z axis
        // The z axis continues to be the same.

        // printf("h = %f, v = %f\n", horizontal, vertical);

        // g is from Ground
        v.x = cosf(horizontal) * r;
        v.y = sinf(horizontal) * r;
    }

    camera = target + v * normalDistance;
}