
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

    time_t accumulatedTime = 0;
    
    time_t timePerLight = 500;

    int currentLight = 0;

    int lightCount = 8;

public:

    airstrip_t(const point3f& start, const point3f& end, float width);

    void update(int millis);
    
    void putLight() const;

    void draw() const;

    void drawMapElement(circle_blueprint_t* blueprint) const override;
};

#endif /* AIRSTRIP_H */

