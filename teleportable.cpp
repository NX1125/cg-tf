
#include <cstdio>

#include "teleportable.h"

void teleportable_t::teleport(float bounds) {
    point3f p = getPosition();
    vector3f v = getVelocity();

    // makes the boundary smaller since the center of the object is considered
    // instead of its circle bounds
    bounds -= getRadius();
    
    // project the position to the ground and compute its distance square
    // to the origin which is where the arena is centered at.
    if (p.x * p.x + p.y * p.y >= bounds * bounds &&
            v.x * p.x + v.y * p.y > 0) {
        printf("Teleporting %s\n", getName());
        // the circle hit the wall of the arena. It will be teleported 
        // to the other side
        point2d start(p.x, p.y);

        // project the velocity to the ground since we use the 2D version
        // of the arena.
        vector2d proj(v.x, v.y);
        proj.normalize();
        // (0, 0) is the center of the arena which is where it is centered
        // at.
        point2d c = closestPointFromLineToPoint(start, proj, point2d(0, 0));

        // get the distance from the closest point to the position
        vector2d r = start - c;

        p.x = c.x - r.x;
        p.y = c.y - r.y;

        setPosition(p);
    }
}
