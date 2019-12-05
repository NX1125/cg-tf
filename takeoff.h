/* 
 * File:   takeoff_t.h
 * Author: gg
 *
 * Created on 4 de dezembro de 2019, 23:27
 */

#ifndef TAKEOFF_T_H
#define TAKEOFF_T_H

#include "shapes.h"

/**
 * A class that helps with the takeoff of the airplane. The takeoff was
 * previously called lifter, but now is called Takeoff. In the previous version,
 * the takeoff consisted of the airplane moving from the start of the airstrip
 * to the end of it, by increasing the radius and increasing the velocity as
 * well. In 3D though, we wont need the increase of radius, but we need a
 * vertical velocity that makes the airplane go up. The airplane translates to
 * the middle of the airstrip with a constant acceleration. The middle is the
 * point that the airplane takeoff the land and starts flying. There are two
 * sections then for the takeoff.
 */
class takeoff_t {
private:

    /**
     * The starting point of the airstrip.
     */
    point3f start;
    /**
     * The end point of the airstrip.
     */
    point3f end;

    /**
     * A vector on ground that is the normal of the airstrip.
     */
    vector3f normal;

    /**
     * The length of the airstrip. This is the distance between the starting and
     * ending points of the airstrip.
     */
    float length;

    /**
     * The height that the airplane goes until the end of the takeoff.
     */
    float height;

    /**
     * The acceleration on the xy plane of the airplane when taking off. This is
     * a constant magnitude acceleration on the xy plane of the airplane.
     */
    float acceleration;

    /**
     * The total time that the airplane must complete the takeoff. The time
     * measured between the start and end of the takeoff must not be greater
     * than this time.
     */
    unsigned int timeToComplete;

    /**
     * The time since the start of the current takeoff. 
     */
    unsigned int currentTime;

    /**
     * The current position of the target that takes off.
     */
    point3f position;

    /**
     * The forward direction of the airplane. When on ground, this has the same
     * normal as the airstrip, but when flying it has a z component.
     */
    vector3f direction;

    /**
     * Whether the takeoff is complete.
     */
    bool completed;

public:

    takeoff_t(const point3f& start, const point3f& end,
            float height, unsigned int timeToComplete);

    void reset();

    void run(int time);

private:
    void setGround(float k);

    void setTakeoff(float k);

    float getTakeoffHeight(float x) const;

    float getTakeoffTangentZ(float x) const;
};

#endif /* TAKEOFF_T_H */

