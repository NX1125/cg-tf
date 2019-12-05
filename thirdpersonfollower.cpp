/* 
 * File:   thirdpersonfollower.cpp
 * Author: gg
 * 
 * Created on 4 de dezembro de 2019, 17:23
 */

#include "thirdpersonfollower.h"

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

    float horizontal = M_PI + atan2(v.y, v.x) + dx * horizontalFactor;
    float vertical = atan2(-v.z, vector3f(v.x, v.y, 0).length());

    // printf("h = %f, v = %f\n", horizontal, vertical);
    
    // printf("h = %f, v = %f\n", horizontal, vertical);
    // printf("v = (%f, %f, %f)\n", v.x, v.y, v.z);

    // if (v.z > 0) {
    //     vertical = -vertical;
    // }

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