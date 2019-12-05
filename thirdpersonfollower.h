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
    float followFactor = 0.3;
    
    int previousMousePositionX;
    int previousMousePositionY;
    
    float horizontalFactor = 0.01f;
    float verticalFactor = 0.01f;
public:

    third_person_follower_t(point3f* target, float normalDistance);
    
    void lookAt();

    void setMousePressingPosition(int x, int y);
    
    /**
     * Called when the mouse were moved. The camera rotates around the target
     * with horizontal and vertical movememnts.
     */
    void mouseDragged(int x, int y);
    
    void follow(float dt);
    
private:
    
    void move(float dx, float dy);
};

#endif /* THIRD_PERSON_FOLLOWER_T_H */

