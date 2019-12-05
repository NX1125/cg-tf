/* 
 * File:   game.h
 * Author: gg
 *
 * Created on 5 de dezembro de 2019, 01:34
 */

#ifndef GAME_H
#define GAME_H

#include <GL/glut.h>

#include "shapes.h"
#include "thirdpersonfollower.h"
#include "settings.h"
#include "resetlistener.h"
#include "wfobj.h"
#include "arena.h"

enum Behaviour {
    ON_GROUND, TAKING_OFF, CONTROLLING, GAME_OVER
};

/**
 * A static class, just to not use global variables, as asked in a previous TC.
 */
class Game {
public:

    static int sWidth;
    static int sHeight;

    static third_person_follower_t* sFollower;
    static point3f sTarget;

    static bool sFollowerMouseEnabled;

    static Behaviour sBehaviour;

    static vector<reset_listener_t*> sResetListeners;
    
    static wf_object_t* sHouseModel;
    
    static arena_t* sArena;

    static void init(app_settings* settings);

    static void loadModels();

    // Extracted from:
    // https://www.opengl.org/archives/resources/code/samples/glut_examples/examples/cube.c
    // Will delete after the third-person-camera is done
    static GLfloat sLightDiffuse[];
    static GLfloat sLightPosition[];
    static GLfloat normals[6][3];
    static GLint faces[6][4];
    static GLfloat vertices[8][3]; /* Will be filled in with X,Y,Z vertexes. */
    static GLfloat color[6][3];

    static void drawBox(void);

    static void display();

    static void mouseDragged(int x, int y);

    static void mouseMoved(int x, int y);

    static void mouseButtonEvent(int button, int state, int x, int y);

    static void mousePressed(int button, int x, int y);

    static void mouseReleased(int button, int x, int y);

    static void reshape(int width, int height);
    
    static void idle();

    static void keyPress(unsigned char key, int x, int y);

    static void reset();

    static void addResetListener(reset_listener_t* l);
};


#endif /* GAME_H */

