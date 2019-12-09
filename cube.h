/* 
 * File:   cube.h
 * Author: 2019202344
 *
 * Created on 6 de Dezembro de 2019, 18:01
 */

#ifndef CUBE_H
#define CUBE_H

#include "wfobj.h"

// https://www.opengl.org/archives/resources/code/samples/glut_examples/examples/cube.c

extern wf_object_t* sCubeModel;

void loadCube(wf_object_loader_t& loader);

void drawCube(void);

#endif /* CUBE_H */

