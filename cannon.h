
/* 
 * File:   cannon.h
 * Author: 2019202344
 *
 * Created on 6 de Dezembro de 2019, 19:54
 */

#ifndef CANNON_H
#define CANNON_H

#include "shapes.h"
#include "wfobj.h"


class cannon_t {
private:
    
    static wf_object_t* sModel;
    
    /**
     * Position relative to the airplane.
     */
    vector3f offset;
    
    float horizontal = 0;
    float vertical = 0;
    
    float horizontalFactor = 0.3f;
    float verticalFactor = 0.3f;
public:
    
    cannon_t(const vector3f& position);
    
    void draw();
    
    void setInputAxis(float x, float y);
};

#endif /* CANNON_H */

