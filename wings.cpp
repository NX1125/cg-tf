
/* 
 * File:   wings.cpp
 * Author: 2019202344
 * 
 * Created on 9 de Dezembro de 2019, 20:10
 */

#include "wings.h"

void wing_t::update(int millis) {
    accumulatedTime += millis;
    if (accumulatedTime > timeToFlap) {
        accumulatedTime = 0;
        upwards = !upwards;
    }

    const float min = 5;
    const float max = 80;

    float t = accumulatedTime / (float) timeToFlap;
    if (upwards) t = 1 - t;

    angle = (max - min) * t + min;
}
