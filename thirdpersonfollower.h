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
    point3f* target = NULL;
    point3f camera;
    
    float normalDistance = 1;
    float followFactor = 0.5;
    
    int previousMousePositionX;
    int previousMousePositionY;
    
    float horizontalFactor = 0.01f;
    float verticalFactor = 0.01f;
public:

    third_person_follower_t(point3f* target, float normalDistance) :
    target(target), normalDistance(normalDistance) {
        camera = *target;
        camera.x += normalDistance;
    }
    
    void lookAt() {
        gluLookAt(camera.x, camera.y, camera.z,
                target->x, target->y, target->z,
                0,0,1);
    }

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
    
    void move(float dx, float dy);
};

#endif /* THIRD_PERSON_FOLLOWER_T_H */

