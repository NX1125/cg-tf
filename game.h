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
#include "enemybase.h"
#include "flyingenemy.h"

enum Camera {
    COCKPIT, CANNON_VIEW, THIRD_PERSON_CAMERA, UP_VIEW, SIDE_VIEW,
    TAKEOFF_FUNCTION_VIEW
};

/**
 */
class Game : public base_death_t {
private:
    // As stated in spec, the size of the window is initially 500x500.
    int width;
    int height;

    GLfloat lightDiffuse[4] = {1.0, 0.0, 0.0, 1.0}; /* Red diffuse light. */
    GLfloat sLightPosition[4] = {1.0, 1.0, 1.0, 0.0}; /* Infinite light location. */

    third_person_follower_t* follower = NULL;

    bool followerMouseEnabled = false;

    vector<reset_listener_t*> sResetListeners;

    arena_t* arena = NULL;

    player_t* player = NULL;

    stopwatch_t* watch = NULL;

    time_t sAccumulatedTime = 0;

    Camera cameraView = Camera::THIRD_PERSON_CAMERA;

    const float normalDistance = 100;

    std::vector<enemy_base_t*> bases;
    std::vector<flying_enemy_t*> enemies;

    projectile_manager_t* manager;

    unsigned int score = 0;
    string scoreText = string("0");

    time_t timeToEnemyFire;

    time_t timeSinceLastEnemyFire = 0;

public:

    Game(app_settings* settings);

private:

    void loadModels();

    void createBases(vector<simple_svg_circle*>& bases);

    void createEnemies(vector<simple_svg_circle*>& enemies, float height, float initialVelocity);

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

    void onBaseDeath() override;
};


#endif /* GAME_H */

