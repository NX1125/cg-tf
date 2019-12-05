/* 
 * File:   thirdpersonfollower.cpp
 * Author: gg
 * 
 * Created on 4 de dezembro de 2019, 17:23
 */

#include "thirdpersonfollower.h"

third_person_follower_t::third_person_follower_t(point3f* target, float normalDistance) :
target(target), normalDistance(normalDistance) {
    camera = *target;
    camera.x += normalDistance;
}

void third_person_follower_t::lookAt() {
    gluLookAt(camera.x, camera.y, camera.z,
            target->x, target->y, target->z,
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

    move(dx, dy);

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
    vector3f v = *target - camera;

    float distance = v.normalize();

    // convert the target-camera vector to angles.
    float horizontal = M_PI + atan2(v.y, v.x) + dx * horizontalFactor;
    float vertical = atan2(-v.z, vector3f(v.x, v.y, 0).length());

    vertical -= dy * verticalFactor;

    horizontal = clampAngle(horizontal);

    // clamp the vertical angle to -60° and 60°
    const float maxVerticalAngle = 60 * M_PI / 180.0f;
    if (vertical < -maxVerticalAngle) {
        vertical = -maxVerticalAngle;
    } else if (vertical > maxVerticalAngle) {
        vertical = maxVerticalAngle;
    }

    // rotate <1,0,0> by vertical around the y axis
    float x = cosf(vertical);
    float z = sinf(vertical);

    // rotate <x, 0, z> by horizontal around the z axis
    // The z axis continues to be the same.

    // printf("h = %f, v = %f\n", horizontal, vertical);

    // g is from Ground
    float g = x;
    x = cosf(horizontal) * g;
    float y = sinf(horizontal) * g;

    // <x, y, z> is the vector of the camera
    camera = *target + vector3f(x, y, z) * distance;
    // printf("camera = (%f, %f, %f)\n", camera.x, camera.y, camera.z);
}

void third_person_follower_t::follow(float dt) {
    vector3f v = *target - camera;
    float length = v.normalize();
    
    length = length * (1 - followFactor) + normalDistance * followFactor;
    
    camera = *target - v * length;
}