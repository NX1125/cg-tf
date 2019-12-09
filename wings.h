
/* 
 * File:   wings.h
 * Author: 2019202344
 *
 * Created on 9 de Dezembro de 2019, 20:10
 */

#ifndef WINGS_H
#define WINGS_H

#include <ctime>

class wing_t {
private:

    float angle = 0;

    time_t timeToFlap = 125;
    time_t accumulatedTime = 0;

    bool upwards = false;

public:

    void update(int millis);

    float getAngle() const {
        return angle;
    }
};

#endif /* WINGS_H */

