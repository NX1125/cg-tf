

#include "game.h"


GLfloat Game::light_diffuse[] = {1.0, 0.0, 0.0, 1.0}; /* Red diffuse light. */
GLfloat Game::light_position[] = {1.0, 1.0, 1.0, 0.0}; /* Infinite light location. */
GLfloat Game::normals[6][3] = {/* Normals for the 6 faces of a cube. */
    {-1.0, 0.0, 0.0},
    {0.0, 1.0, 0.0},
    {1.0, 0.0, 0.0},
    {0.0, -1.0, 0.0},
    {0.0, 0.0, 1.0},
    {0.0, 0.0, -1.0}
};
GLint Game::faces[6][4] = {/* Vertex indices for the 6 faces of a cube. */
    {0, 1, 2, 3},
    {3, 2, 6, 7},
    {7, 6, 5, 4},
    {4, 5, 1, 0},
    {5, 6, 2, 1},
    {7, 4, 0, 3}
};
GLfloat Game::vertices[8][3]; /* Will be filled in with X,Y,Z vertexes. */
GLfloat Game::color[6][3] = {
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1},
    {1, 1, 0},
    {0, 1, 1},
    {1, 0, 1},
};

third_person_follower_t* Game::sFollower = NULL;
point3f Game::sTarget;

Behaviour Game::sBehaviour = Behaviour::ON_GROUND;
bool Game::sFollowerMouseEnabled = false;
vector<reset_listener_t*> Game::sResetListeners ;

wf_object_t* Game::sHouseModel = NULL;


// As stated in spec, the size of the window is initially 500x500.
int Game::sWidth = 500;
int Game::sHeight = 500;

