
#include "game.h"
#include "player.h"

GLfloat Game::sLightDiffuse[] = {1.0, 0.0, 0.0, 1.0}; /* Red diffuse light. */
GLfloat Game::sLightPosition[] = {1.0, 1.0, 1.0, 0.0}; /* Infinite light location. */
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

bool Game::sFollowerMouseEnabled = false;

vector<reset_listener_t*> Game::sResetListeners;

arena_t* Game::sArena = NULL;

player_t* Game::sPlayer = NULL;

stopwatch_t* Game::sWatch = NULL;

time_t Game::sAccumulatedTime = 0;

// As stated in spec, the size of the window is initially 500x500.
int Game::sWidth = 500;
int Game::sHeight = 500;

Camera Game::sCameraView = Camera::UP_VIEW;

const float Game::NORMAL_DISTANCE = 50;

void Game::init(app_settings* settings) {
    glClearColor(0, 0, 0, 0);

    vertices[0][0] = vertices[1][0] = vertices[2][0] = vertices[3][0] = -1;
    vertices[4][0] = vertices[5][0] = vertices[6][0] = vertices[7][0] = 1;
    vertices[0][1] = vertices[1][1] = vertices[4][1] = vertices[5][1] = -1;
    vertices[2][1] = vertices[3][1] = vertices[6][1] = vertices[7][1] = 1;
    vertices[0][2] = vertices[3][2] = vertices[4][2] = vertices[7][2] = 1;
    vertices[1][2] = vertices[2][2] = vertices[5][2] = vertices[6][2] = -1;

    glLightfv(GL_LIGHT0, GL_DIFFUSE, sLightDiffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, sLightPosition);

    glEnable(GL_LIGHT0);

    glEnable(GL_DEPTH_TEST);

    // as stated in the spec, the height of the arena is 8 times the player's
    // diameter. We have the radius, that's why 2 * 8
    sArena = new arena_t(settings->player->radius * 2 * 8, settings->arena->radius);

    simple_svg_line* airstrip = settings->airstrip;

    point3f start(airstrip->x1, airstrip->y1, 0);
    point3f end(airstrip->x2, airstrip->y2, 0);

    takeoff_t* takeoff = new takeoff_t(start, end, sArena->getHeight() / 2, 4000);

    sPlayer = new player_t(takeoff, settings->player->radius);

    sArena->setAirstrip(new airstrip_t(start, end, sPlayer->getRadius() * 2));

    sFollower = new third_person_follower_t(/*point3f(0,0,0)*/
            sPlayer->getPosition(), NORMAL_DISTANCE);
    sFollower->setAngle(0, -20 * M_PI / 180.0);

    loadModels();

    sWatch = new stopwatch_t();

    sPlayer->setVelocityFactor(settings->vel);
}

void Game::loadModels() {
    wf_object_loader_t loader;

    player_t::sInit(loader);
}

void Game::drawBox(void) {
    int i;

    for (i = 0; i < 6; i++) {
        glColor3f(color[i][0], color[i][1], color[i][2]);
        glBegin(GL_QUADS);
        glNormal3fv(&normals[i][0]);
        glVertex3fv(&vertices[faces[i][0]][0]);
        glVertex3fv(&vertices[faces[i][1]][0]);
        glVertex3fv(&vertices[faces[i][2]][0]);
        glVertex3fv(&vertices[faces[i][3]][0]);
        glEnd();
    }
}

void Game::display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    switch (sCameraView) {
        case Camera::UP_VIEW:
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            glOrtho(-sArena->getRadius(), sArena->getRadius(), -sArena->getRadius(), sArena->getRadius(), -sArena->getHeight() * 2, sArena->getHeight() * 2);
            break;
        case Camera::THIRD_PERSON_CAMERA:
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(/* field of view in degree */ 40.0,
                    /* aspect ratio */ 1.0,
                    /* Z near */ 1.0, /* Z far */ 500.0);

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            sFollower->lookAt();
            break;
    }


    glEnable(GL_LIGHTING);

    glColor3f(1, 0, 0);
    // Draw a cube for reference to the camera
    // drawBox();

    sPlayer->draw();

    glDisable(GL_LIGHTING);

    // TODO Update camera depending of the current type
    sArena->draw();
    // TODO Draw each flying enemy
    // TODO Draw each base
    // TODO Draw projectiles (bombs and bullets)

    glutSwapBuffers();
}

void Game::mouseDragged(int x, int y) {
    if (sFollowerMouseEnabled) {
        sFollower->mouseDragged(x, y);
        glutPostRedisplay();
    }
}

void Game::mouseMoved(int x, int y) {
    // TODO Move cannon of player
}

void Game::mouseButtonEvent(int button, int state, int x, int y) {
    switch (state) {
        case GLUT_DOWN:
            mousePressed(button, x, y);
            break;
        case GLUT_UP:
            mouseReleased(button, x, y);
            break;
    }
}

void Game::mousePressed(int button, int x, int y) {
    if (button == GLUT_RIGHT_BUTTON &&
            sPlayer->getBehaviour() == Behaviour::ON_GROUND) {
        sFollower->setMousePressingPosition(x, y);
        sFollowerMouseEnabled = true;
    }
}

void Game::mouseReleased(int button, int x, int y) {
    sFollowerMouseEnabled = false;
}

void Game::reshape(int width, int height) {
    sWidth = width;
    sHeight = height;

    glViewport(0, 0, width, height);

    printf("Reshape: (%d, %d)\n", width, height);
}

void Game::idle() {
    sWatch->mark();

    int time = sWatch->getMarkedElaspedTimeMillis();

    if (time <= 10) {
        return;
    }

    // printf("Time: %d ms\n", time);

    sPlayer->update(time);

    if (sPlayer->canTeleport()) {
        sPlayer->teleport(sArena->getRadius());
    }

    sFollower->setTarget(sPlayer->getPosition());
    sFollower->follow(time / 1000.0f);

    sAccumulatedTime += time;

    if (sAccumulatedTime * 30 > 1000) {
        glutPostRedisplay();
        sAccumulatedTime = 0;
    }

    sWatch->setMarkAsOffset();
}

void Game::keyPressed(unsigned char key, int x, int y) {
    switch (key) {
        case 'r':
            reset();
            return;
        case '1':
            printf("Changing to cockpit view\n");
            sCameraView = Camera::COCKPIT;
            break;
        case '2':
            printf("Changing to cannon view\n");
            sCameraView = Camera::CANNON_VIEW;
            break;
        case '3':
            printf("Changing to third person view\n");
            sCameraView = Camera::THIRD_PERSON_CAMERA;
            break;
        case '4':
            printf("Changing to up view\n");
            sCameraView = Camera::UP_VIEW;
            break;
        default:
            sPlayer->keyPress(key);
            return ;
    }
    glutPostRedisplay();
}

void Game::keyReleased(unsigned char key, int x, int y) {
    sPlayer->keyRelease(key);
}

void Game::reset() {
    printf("Resetting game\n");
    printf("Resetting listeners");
    for (reset_listener_t* l : sResetListeners) {
        l->reset();
    }
}

void Game::addResetListener(reset_listener_t* l) {
    sResetListeners.push_back(l);
}

