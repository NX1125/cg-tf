
/* 
 * File:   airstrip.h
 * Author: 2019202344
 *
 * Created on 6 de Dezembro de 2019, 13:20
 */

#ifndef AIRSTRIP_H
#define AIRSTRIP_H

#include "shapes.h"
#include "minimapitem.h"


class airstrip_t : public mini_map_item_t {
private:
    
    point2d start;
    
    point2d end;
    
    float width;
public:
    
    airstrip_t(const point3f& start, const point3f& end, float width);

    void draw() const;
    
    void drawMapElement(circle_blueprint_t* blueprint) const override;
};

#endif /* AIRSTRIP_H */

