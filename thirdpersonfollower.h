/* 
 * File:   thirdpersonfollower.h
 * Author: gg
 *
 * Created on 4 de dezembro de 2019, 17:23
 */

#ifndef THIRD_PERSON_FOLLOWER_T_H
#define THIRD_PERSON_FOLLOWER_T_H

#include <GL/glu.h>
#include <cstdio>

#include "shapes.h"

/**
 * A class that has a camera point following a target and that can orbit around
 * the target with mouse movements.
 */
class third_person_follower_t {
private:

    float normalDistance = 1;

    int previousMousePositionX;
    int previousMousePositionY;

    float horizontalFactor = 0.01f;
    float verticalFactor = 0.01f;

    float maxVerticalAngle = 60 * M_PI / 180.0f;

    float horizontal = 0;
    float vertical = 0;
public:

    third_person_follower_t(const point3f& target, float normalDistance);

    void reset() {
        horizontal = 0;
        vertical = 0;
    }

    void lookAt(const point3f& target, float h, float v);

    void lookAtDebug();

    void setMousePressingPosition(int x, int y);

    /**
     * Called when the mouse were moved. The camera rotates around the target
     * with horizontal and vertical movememnts.
     */
    void mouseDragged(int x, int y);

    void follow(float dt);

    /**
     * Repositions the camera with horizontal and vertical angles, and a
     * distance between the camera and the target.
     * 
     * Both angles are clampped to between the intervals.
     * 
     * @param horizontal An angle in radians
     * @param vertical   An angle in radians
     * @param distance   The new distance between the target and the camera
     */
    void setAngle(float horizontal, float vertical, float distance);

    /**
     * Overloaded method for <code>setAngle(float, float, float)</code> with the
     * distance being the current distance.
     * 
     * @param horizontal An angle in radians
     * @param vertical   An angle in radians
     */
    void setAngle(float horizontal, float vertical);

    void rotate(vector3f& v) const;

    vector3f getCamera(float h, float v) const;

    float getNormalDistance() const {
        return normalDistance;
    }

private:

    void move(float dx, float dy);
};

#endif /* THIRD_PERSON_FOLLOWER_T_H */

