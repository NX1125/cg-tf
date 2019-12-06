/* 
 * File:   airstrip.cpp
 * Author: 2019202344
 * 
 * Created on 6 de Dezembro de 2019, 13:20
 */

#include <GL/gl.h>

#include "airstrip.h"
#include "shapes.h"

airstrip_t::airstrip_t(const point3f& start, const point3f& end, float width) :
start(start.x, start.y), end(end.x, end.y), width(width) {
}

void airstrip_t::draw() {
    glColor3f(0,0,0);
    glBegin(GL_LINES);
    glVertex3d(start.x, start.y, 1);
    glVertex3d(end.x, end.y, 1);
    glEnd();
}
