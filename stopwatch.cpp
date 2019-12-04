/* 
 * File:   stopwatch.cpp
 * Author: gg
 * 
 * Created on October 7, 2019, 11:34 AM
 */

#include <GL/freeglut_std.h>

#include "stopwatch.h"

stopwatch_t::~stopwatch_t() {
}

int stopwatch_t::currentTimeMillis() {
    return glutGet(GLUT_ELAPSED_TIME);
}

