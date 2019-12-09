/* 
 * File:   arena.h
 * Author: gg
 *
 * Created on 5 de dezembro de 2019, 20:07
 */

#ifndef ARENA_H
#define ARENA_H

#include "shapes.h"
#include "airstrip.h"
#include "minimapitem.h"
#include "wfobj.h"

/**
 * The arena is the environment that the player plays the game. In here, there
 * are decorations, bases for enemies, the airstrip, a roof, a ground and the 
 * walls.
 */
class arena_t : public mini_map_item_t {
private:

    /**
     * The circle that is used to create the roof and the ground. Both uses the
     * same circle and these are also used for the walls of the arena.
     * 
     * The roof is rotate differently of the ground, since it need to have the
     * faces of the triangle downwards. The rotation makes it downwards.
     * 
     * The walls are created with a triangle strip between the roof and ground
     * points.
     */
    circle_blueprint_t* blueprint;

    /**
     * The distance from the ground to the roof. 
     */
    float height;

    /**
     * The radius of the arena. This is the same for the roof and for the
     * ground.
     */
    float radius;

    float radiusSqr;

    airstrip_t* airstrip = NULL;
    
    wf_object_t* ground = NULL;

public:

    arena_t(float height, float radius, wf_object_loader_t& loader);

    /**
     * The drawing of an arena counts the bases of the enemies, decorations such
     * as trees, the airstrip, the walls, the roof and the ground.
     * 
     * Enemy bases that are dead are not painted.
     */
    void draw();

    void setAirstrip(airstrip_t* airstrip) {
        this->airstrip = airstrip;
    }

    airstrip_t* getAirstrip() const {
        return airstrip;
    }

    float getHeight() const {
        return height;
    }

    float getRadius() const {
        return radius;
    }

    float getRadiusSqr() const {
        return radiusSqr;
    }

    void drawMapElement(circle_blueprint_t* blueprint) const override;

private:

    /**
     * Translates to the correct z, does the invertion of faces and draws the
     * roof simirlarly to the ground.
     */
    void drawRoof();

    /**
     * Draws the ground on the current z of the matrix.
     */
    void drawGround();
};

#endif /* ARENA_H */

