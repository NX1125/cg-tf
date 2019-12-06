
#include "game.h"
#include "player.h"
#include "cube.h"

Game::Game(app_settings* settings) {
    glClearColor(0, 0, 0, 0);

    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, sLightPosition);

    glEnable(GL_LIGHT0);

    glEnable(GL_DEPTH_TEST);

    // as stated in the spec, the height of the arena is 8 times the player's
    // diameter. We have the radius, that's why 2 * 8
    arena = new arena_t(settings->player->radius * 2 * 8, settings->arena->radius);

    simple_svg_line* airstrip = settings->airstrip;

    point3f start(airstrip->x1, airstrip->y1, 0);
    point3f end(airstrip->x2, airstrip->y2, 0);

    takeoff_t* takeoff = new takeoff_t(start, end, arena->getHeight() / 5, 4000);

    player = new player_t(takeoff, settings->player->radius);

    arena->setAirstrip(new airstrip_t(start, end, player->getRadius() * 2));

    follower = new third_person_follower_t(/*point3f(0,0,0)*/
            player->getPosition(), normalDistance);
    follower->setAngle(0, 20 * M_PI / 180.0);

    loadModels();

    watch = new stopwatch_t();

    player->setVelocityFactor(settings->vel);

    manager = new projectile_manager_t();

    createBases(settings->groundEnemies);
    createEnemies(settings->flyingEnemies, arena->getHeight() / 2, settings->eVel * takeoff->getFinalVelocity());
    
    player->setManager(manager);
}

void Game::createBases(vector<simple_svg_circle*>& bases) {
    for (simple_svg_circle* enemy : bases) {
        enemy_base_t* b = new enemy_base_t(point3f(enemy->cx, enemy->cy, enemy->radius), enemy->radius);
        manager->addObstacles(b, false);
        this->bases.push_back(b);
    }
}

void Game::createEnemies(vector<simple_svg_circle*>& enemies, float height, float velocity) {
    for (simple_svg_circle* enemy : enemies) {
        point3f p(enemy->cx, enemy->cy, height);
        flying_enemy_t* e = new flying_enemy_t(p, enemy->radius);
        manager->addObstacles(e, true);
        e->setInitialVelocity(velocity);
        this->enemies.push_back(e);
    }
}

void Game::loadModels() {
    wf_object_loader_t loader;

    player_t::sInit(loader);
}

void Game::display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    switch (cameraView) {
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

            glOrtho(-arena->getRadius(), arena->getRadius(), -arena->getRadius(), arena->getRadius(), -arena->getHeight() * 2, arena->getHeight() * 2);
            break;
        case Camera::SIDE_VIEW:
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glOrtho(-arena->getRadius(), arena->getRadius(), -arena->getRadius(), arena->getRadius(), -arena->getHeight() * 2, arena->getHeight() * 2);
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
            follower->lookAt();
            break;
        case Camera::CANNON_VIEW:
        case Camera::COCKPIT:
            break;
    }

    // glEnable(GL_LIGHTING);

    player->draw();

    // glDisable(GL_LIGHTING);

    arena->draw();
    for (enemy_base_t* base : bases) {
        base->transformAndDraw();
    }
    for (flying_enemy_t* enemy : enemies) {
        enemy->transformAndDraw();
    }
    // TODO Draw projectiles (bombs and bullets)
    manager->draw();

    glutSwapBuffers();
}

void Game::mouseDragged(int x, int y) {
    if (followerMouseEnabled) {
        follower->mouseDragged(x, y);
        glutPostRedisplay();
    }
}

void Game::mouseMoved(int x, int y) {
    //    if (player->getBehaviour() == Behaviour::CONTROLLING) {
    player->setCannonAxis(x / (GLfloat) width, y / (GLfloat) height);
    //    }
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
            player->getBehaviour() == Behaviour::ON_GROUND) {
        follower->setMousePressingPosition(x, y);
        followerMouseEnabled = true;
    } else {
        player->mousePress(button);
    }
}

void Game::mouseReleased(int button, int x, int y) {
    followerMouseEnabled = false;
}

void Game::reshape(int width, int height) {
    this->width = width;
    this->height = height;

    glViewport(0, 0, width, height);

    printf("Reshape: (%d, %d)\n", width, height);
}

void Game::idle() {
    watch->mark();

    int time = watch->getMarkedElaspedTimeMillis();

    if (time <= 10) {
        return;
    }

    // printf("Time: %d ms\n", time);

    player->update(time);
    player->clipZ(arena->getHeight());

    manager->update(time);
    
    manager->removeOutsideOfArena(arena->getRadius(), arena->getHeight());
    
    for (flying_enemy_t* enemy : enemies) {
        enemy->update(time);
        enemy->clipZ(arena->getHeight());
    }

    if (player->canDie()) {
        for (flying_enemy_t* enemy : enemies) {
            if (!enemy->isDead() && enemy->overlaps(player)) {
                printf("The player just hit an enemy while flying\n");
                enemy->kill();
                player->kill();
                break;
            }
        }
    }

    for (flying_enemy_t* enemy : enemies) {
        enemy->teleport(arena->getRadius());
    }

    if (player->canTeleport()) {
        point3f p = player->getPosition();
        if (player->teleport(arena->getRadius())) {
            follower->setCamera(follower->getCamera() + (player->getPosition() - p));
        }
    }

    follower->setTarget(player->getPosition());
    follower->follow(time / 1000.0f);

    sAccumulatedTime += time;

    if (sAccumulatedTime * 30 > 1000) {
        glutPostRedisplay();
        sAccumulatedTime = 0;
    }

    watch->setMarkAsOffset();
}

void Game::keyPressed(unsigned char key, int x, int y) {
    switch (key) {
        case 'r':
            reset();
            return;
        case '1':
            printf("Changing to cockpit view\n");
            cameraView = Camera::COCKPIT;
            break;
        case '2':
            printf("Changing to cannon view\n");
            cameraView = Camera::CANNON_VIEW;
            break;
        case '3':
            printf("Changing to third person view\n");
            cameraView = Camera::THIRD_PERSON_CAMERA;
            break;
        case '4':
            printf("Changing to up view\n");
            cameraView = Camera::UP_VIEW;
            break;
        case '5':
            printf("Changing to takeoff function view\n");
            cameraView = Camera::TAKEOFF_FUNCTION_VIEW;
            break;
        case '6':
            printf("Changing to side view\n");
            cameraView = Camera::SIDE_VIEW;
            break;
        default:
            player->keyPress(key);
            return;
    }
    glutPostRedisplay();
}

void Game::keyReleased(unsigned char key, int x, int y) {
    player->keyRelease(key);
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

