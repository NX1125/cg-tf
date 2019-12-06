
/* 
 * File:   airstrip.h
 * Author: 2019202344
 *
 * Created on 6 de Dezembro de 2019, 13:20
 */

#ifndef AIRSTRIP_H
#define AIRSTRIP_H

#include "shapes.h"


class airstrip_t {
private:
    
    point2d start;
    
    point2d end;
    
    float width;
public:
    
    airstrip_t(const point3f& start, const point3f& end, float width);

    void draw();
};

#endif /* AIRSTRIP_H */

