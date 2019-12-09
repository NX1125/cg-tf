
/* 
 * File:   propeller.h
 * Author: 2019202344
 *
 * Created on 6 de Dezembro de 2019, 20:15
 */

#ifndef PROPELLER_H
#define PROPELLER_H

#include "shapes.h"
#include "wfobj.h"

class propeller_t {
private:

    static wf_object_t* sModel;

    float angle = 0;

    vector3f offset;

    float angularVelocity = 0;

    float factor = 360.0f;

public:
    propeller_t(vector3f offset);

    void transformAndDraw(float s)const;

    void update(int millis);

    void setMagnitude(float v);

    void setScaleFactor(float scale);

    static void init(wf_object_loader_t& loader);
};

#endif /* PROPELLER_H */

