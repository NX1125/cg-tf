/* 
 * File:   minimapitem.h
 * Author: gg
 *
 * Created on 8 de dezembro de 2019, 12:17
 */

#ifndef MINIMAPITEM_H
#define MINIMAPITEM_H

#include "shapes.h"


/**
 * An interface for elements that can be drawn on screen.
 */
class mini_map_item_t {
public:
    virtual void drawMapElement(circle_blueprint_t* blueprint) const = 0;
};

#endif /* MINIMAPITEM_H */

