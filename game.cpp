
#include "game.h"
#include "player.h"
#include "cube.h"

Game::Game(app_settings* settings) {
    glClearColor(0, 0, 0, 0);

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

    takeoff_t* takeoff = new takeoff_t(start, end, sArena->getHeight() / 5, 4000);

    sPlayer = new player_t(takeoff, settings->player->radius);

    sArena->setAirstrip(new airstrip_t(start, end, sPlayer->getRadius() * 2));

    sFollower = new third_person_follower_t(/*point3f(0,0,0)*/
            sPlayer->getPosition(), NORMAL_DISTANCE);
    sFollower->setAngle(0, 20 * M_PI / 180.0);

    loadModels();

    sWatch = new stopwatch_t();

    sPlayer->setVelocityFactor(settings->vel);
}

void Game::loadModels() {
    wf_object_loader_t loader;

    player_t::sInit(loader);
}

void Game::display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    switch (sCameraView) {
        case Camera::TAKEOFF_FUNCTION_VIEW:
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glOrtho(0, 1, 0, 1, 1, -1);
        {
            int n = 20;

            glColor3f(1, 1, 0);
            glBegin(GL_LINE_STRIP);
            {
                for (int i = 0; i <= n; i++) {
                    float x = i / (float) n;
                    float y = takeoff_t::getTakeoffFactor(x);

                    glVertex2d(x, y);
                }
            }
            glEnd();

            glColor3f(0, 1, 0);
            glBegin(GL_LINE_STRIP);
            {
                for (int i = 0; i <= n; i++) {
                    float x = i / (float) n;
                    float y = takeoff_t::getTakeoffTangent(x);

                    glVertex2d(x, y);
                }
            }
            glEnd();

            glColor3f(1, 0, 0);
            glBegin(GL_LINE_STRIP);
            {
                for (int i = 0; i <= n; i++) {
                    float x = i / (float) n;
                    float y = takeoff_t::getTakeoffTangent(x);
                    y = atanf(y);

                    glVertex2d(x, y);
                }
            }
            glEnd();
        }

            glutSwapBuffers();

            return;
        case Camera::UP_VIEW:
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            glOrtho(-sArena->getRadius(), sArena->getRadius(), -sArena->getRadius(), sArena->getRadius(), -sArena->getHeight() * 2, sArena->getHeight() * 2);
            break;
        case Camera::SIDE_VIEW:
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glOrtho(-sArena->getRadius(), sArena->getRadius(), -sArena->getRadius(), sArena->getRadius(), -sArena->getHeight() * 2, sArena->getHeight() * 2);
            glRotated(-90, 1, 0, 0);
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
    } else {
        sPlayer->mousePress(button);
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
    sPlayer->clipZ(sArena->getHeight());

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
        case '5':
            printf("Changing to takeoff function view\n");
            sCameraView = Camera::TAKEOFF_FUNCTION_VIEW;
            break;
        case '6':
            printf("Changing to side view\n");
            sCameraView = Camera::SIDE_VIEW;
            break;
        default:
            sPlayer->keyPress(key);
            return;
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

