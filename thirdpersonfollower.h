/* 
 * File:   thirdpersonfollower.h
 * Author: gg
 *
 * Created on 4 de dezembro de 2019, 17:23
 */

#ifndef THIRD_PERSON_FOLLOWER_T_H
#define THIRD_PERSON_FOLLOWER_T_H

#include "shapes.h"


/**
 * A class that has a camera point following a target and that can orbit around
 * the target with mouse movements.
 */
class third_person_follower_t {
private:
    point3f* target;
    point3f camera;
    
    float normalDistance;
    float followFactor = 0.5;
    
    float mouseFactor;
    
    int previousMousePositionX;
    int previousMousePositionY;
    
    float horizontalFactor = 1;
    float verticalFactor = 1;
public:

    void setMousePressingPosition(int x, int y) {
        previousMousePositionX = x;
        previousMousePositionY = y;
    }
    
    /**
     * Called when the mouse were moved. The camera rotates around the target
     * with horizontal and vertical movememnts.
     */
    void mouseDragged(int x, int y) {
        int dx = x - previousMousePositionX;
        int dy = y - previousMousePositionY;
        
        move(dx, dy);
        
        previousMousePositionX = x;
        previousMousePositionY = y;
    }
    
private:
    
    void move(float dx, float dy) {
        vector3f v = *target - camera;
        
        float distance = v.normalize();
        
        float horizontal = atan2(v.y, v.x) + dx * horizontalFactor;
        float vertical = vector3f::angle(v, vector3f(v.x, v.y, 0.0f)) + dy * verticalFactor;
        
        horizontal = fmodf(horizontal, M_PI * 2);
        
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
        
        // g is from Ground
        float g = x;
        x = cosf(horizontal) * g;
        float y = sinf(horizontal) * g;
        
        // <x, y, z> is the vector of the camera
        camera = *target + vector3f(x, y, z) * distance;
    }
};

#endif /* THIRD_PERSON_FOLLOWER_T_H */

