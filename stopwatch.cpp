/* 
 * File:   stopwatch.cpp
 * Author: gg
 * 
 * Created on October 7, 2019, 11:34 AM
 */

#include <GL/freeglut_std.h>

#include "stopwatch.h"

stopwatch::~stopwatch() {
}

int stopwatch::currentTimeMillis() {
    return glutGet(GLUT_ELAPSED_TIME);
}

