/* 
 * File:   thirdpersonfollower.cpp
 * Author: gg
 * 
 * Created on 4 de dezembro de 2019, 17:23
 */

#include "thirdpersonfollower.h"

third_person_follower_t::third_person_follower_t(const point3f& target, float normalDistance) {
    this->normalDistance = normalDistance;
}

void third_person_follower_t::lookAt(const point3f& target, float h, float v) {
    point3f camera = target + getCamera(h, v);
    gluLookAt(camera.x, camera.y, camera.z,
            target.x, target.y, target.z,
            0, 0, 1);
}

void third_person_follower_t::lookAtDebug() {
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
    setAngle(
            horizontal + dx * horizontalFactor,
            vertical + dy * verticalFactor
            );
}

void third_person_follower_t::setAngle(float horizontal, float vertical) {
    setAngle(horizontal, vertical, normalDistance);
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

    this->horizontal = horizontal;
    this->vertical = vertical;

    normalDistance = distance;
}

vector3f third_person_follower_t::getCamera(float h, float v) const {
    // rotate <1,0,0> by vertical around the y axis
    float r, z;

    sincosf(vertical + v, &z, &r);

    // rotate <x, 0, z> by horizontal around the z axis
    // The z axis continues to be the same.

    // printf("h = %f, v = %f\n", horizontal, vertical);

    // g is from Ground
    float x, y;

    sincosf(horizontal + h, &y, &x);

    return vector3f(r * x, r * y, z) * normalDistance;
}

void third_person_follower_t::follow(float dt) {
}

void third_person_follower_t::rotate(vector3f& v) const {
    v.rotateY(vertical);
    v.rotateZ(horizontal);

}
