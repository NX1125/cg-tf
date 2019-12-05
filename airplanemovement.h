/* 
 * File:   airplane_movement_t.h
 * Author: gg
 *
 * Created on 4 de dezembro de 2019, 23:31
 */

#ifndef AIRPLANE_MOVEMENT_T_H
#define AIRPLANE_MOVEMENT_T_H

#include <cmath>

#include "shapes.h"

class airplane_movement_t {
private:

    // cached keys

    float left;
    float right;
    float up;
    float down;

    float velocity;
    float velocityAccelerationFactor = 0.05f;

    vector3f velocityAxis;

    point3f previous;
    point3f position;

public:

    void keyPress(unsigned char key) {
        updateKey(key, 1);
    }

    void keyReleased(unsigned char key) {
        updateKey(key, 0);
    }

private:

    void updateKey(unsigned char key, float intensity) {
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
            case '+':
                velocity *= 1 + velocityAccelerationFactor;
                return;
            case '-':
                velocity *= 1 - velocityAccelerationFactor;
                return;
            default:
                return;
        }

        updateAxis();
    }

public:

    void move(float dt) {
        previous = position;
        position = previous + velocityAxis * (dt * velocity);
    }

    float getVerticalAngle() const {
        return up - down;
    }

    float getHorizontalAngle() const {
        return right - left;
    }

private:

    void updateAxis() {
        float v = getVerticalAngle();
        float h = getVerticalAngle();

        float g = cosf(v);

        velocityAxis.x = g * cosf(h);
        velocityAxis.y = g * sinf(h);
        velocityAxis.z = sinf(v);
    }
};

#endif /* AIRPLANE_MOVEMENT_T_H */

