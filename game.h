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
#include "airplanemovement.h"
#include "player.h"
#include "stopwatch.h"
#include "airstrip.h"

enum Camera {
    COCKPIT, CANNON_VIEW, THIRD_PERSON_CAMERA, UP_VIEW, SIDE_VIEW,
    TAKEOFF_FUNCTION_VIEW
};

/**
 */
class Game {
private:
    // As stated in spec, the size of the window is initially 500x500.
    int sWidth;
    int sHeight;

    GLfloat sLightDiffuse[4] = {1.0, 0.0, 0.0, 1.0}; /* Red diffuse light. */
    GLfloat sLightPosition[4] = {1.0, 1.0, 1.0, 0.0}; /* Infinite light location. */

    third_person_follower_t* sFollower = NULL;

    bool sFollowerMouseEnabled = false;

    vector<reset_listener_t*> sResetListeners;

    arena_t* sArena = NULL;

    player_t* sPlayer = NULL;

    stopwatch_t* sWatch = NULL;

    time_t sAccumulatedTime = 0;

    Camera sCameraView = Camera::UP_VIEW;

    const float NORMAL_DISTANCE = 50;

public:

    Game(app_settings* settings);

private:

    void loadModels();

public:

    void display();

    void mouseDragged(int x, int y);

    void mouseMoved(int x, int y);

    void mouseButtonEvent(int button, int state, int x, int y);

    void mousePressed(int button, int x, int y);

    void mouseReleased(int button, int x, int y);

    void reshape(int width, int height);

    void idle();

    void keyPressed(unsigned char key, int x, int y);

    void keyReleased(unsigned char key, int x, int y);

    void reset();

    void addResetListener(reset_listener_t* l);
};


#endif /* GAME_H */

