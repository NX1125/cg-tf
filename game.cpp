
#include "game.h"
#include "player.h"
#include "cube.h"
#include "textrendering.h"
#include "bomb.h"
#include "bullet.h"

#define FIXED_THIRD_PERSON_CAMERA

Game::Game(app_settings* settings) {
    glClearColor(0, 0, 0, 0);

    // as stated in the spec, the height of the arena is 8 times the player's
    // diameter. We have the radius, that's why 2 * 8
    arena = new arena_t(settings->player->radius * 2 * 8, settings->arena->radius);

    simple_svg_line* airstrip = settings->airstrip;

    point3f start(airstrip->x1, airstrip->y1, 0);
    point3f end(airstrip->x2, airstrip->y2, 0);

    takeoff_t* takeoff = new takeoff_t(start, end, arena->getHeight() / 5, 4000);

    player = new player_t(takeoff, settings->player->radius);

    arena->setAirstrip(new airstrip_t(start, end, player->getRadius() * 2));

    normalDistance = player->getRadius() * 8;

    follower = new third_person_follower_t(/*point3f(0,0,0)*/
            player->getPosition(), normalDistance);
    follower->setAngle(0, 20 * M_PI / 180.0);

    loadModels();

    watch = new stopwatch_t();

    player->setVelocityFactor(settings->vel);
    player->setBombListener(this);

    manager = new projectile_manager_t();

    createBases(settings->groundEnemies);
    createEnemies(settings->flyingEnemies, arena->getHeight() / 2, settings->eVel * takeoff->getFinalVelocity());

    player->setManager(manager);

    // A time of 2 seconds for the bomb to fall onto the ground.
    // The time considers that the player is at the roof
    // s(t) = at^2 / 2
    // h = at^2 / 2
    // 2h = at^2
    // 2h / t^2 = a
    const float timeToBombHitGround = 2;

    float a = -2 * arena->getHeight() / 5 / (timeToBombHitGround * timeToBombHitGround);

    printf("The bomb will fall with an acceleration of %f\n", a);

    bomb_t::setGravityAcceleration(a);

    timeToEnemyFire = (time_t) (1000.0f / settings->eBulletF);

    miniMapAux = new circle_blueprint_t(16);

    point3f p = player->getPosition();

    player->setPosition(point3f(30, 30, 30));

    // TODO Add reset listeners 

    glEnable(GL_NORMALIZE);
}

void Game::createBases(vector<simple_svg_circle*>& bases) {
    for (simple_svg_circle* enemy : bases) {
        enemy_base_t* b = new enemy_base_t(point3f(enemy->cx, enemy->cy, enemy->radius), enemy->radius);
        manager->addObstacles(b, false);
        b->setDeathListener(this);
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

    loadCube(loader);
}

void Game::display() {
    // TODO Put light and textures to meshes

    glEnable(GL_CULL_FACE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float yOffset = 200;

    glViewport(0, yOffset, 500, 500);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    switch (cameraView) {
        case Camera::TAKEOFF_FUNCTION_VIEW:
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
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            glOrtho(-arena->getRadius(), arena->getRadius(), -arena->getRadius(), arena->getRadius(), -arena->getHeight() * 2, arena->getHeight() * 2);
            break;
        case Camera::SIDE_VIEW:
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glOrtho(-arena->getRadius(), arena->getRadius(), -arena->getRadius(), arena->getRadius(), -arena->getHeight() * 2, arena->getHeight() * 2);
            glRotated(-90, 1, 0, 0);
            break;
        case Camera::THIRD_PERSON_CAMERA:
            gluPerspective(/* field of view in degree */ 40.0,
                    /* aspect ratio */ 1.0,
                    /* Z near */ 1.0, /* Z far */ 3 * arena->getRadius());

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            if (followerOrbitEnabled) {
                point3f target = player->getPosition();
                target.z += player->getRadius() / 2;
                follower->lookAt(target, player->getHorizontal(),
                        player->getVertical());
            } else {
                vector3f v = player->getDirection(0.2f * M_PI / 2) * follower->getNormalDistance();
                point3f p = player->getPosition();
                gluLookAt(p.x - v.x, p.y - v.y, p.z - v.z,
                        p.x, p.y, p.z, 0, 0, 1);
            }
            break;
        case Camera::CANNON_VIEW:
            gluPerspective(/* field of view in degree */ 40.0,
                    /* aspect ratio */ 1.0,
                    /* Z near */ 1.0, /* Z far */ 3 * arena->getRadius());

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            player->cannonView();
            break;
        case Camera::COCKPIT:
            gluPerspective(/* field of view in degree */ 40.0,
                    /* aspect ratio */ 1.0,
                    /* Z near */ 1.0, /* Z far */ 3 * arena->getRadius());

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            player->cockpitView();
            break;
    }

    glEnable(GL_LIGHTING);

    {
        GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
        GLfloat mat_shininess[] = {50.0};
        GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};

        glShadeModel(GL_SMOOTH);

        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);

        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_DEPTH_TEST);
    }

    drawWorld();

    if (currentBomb != NULL) {
        glViewport(0, 0, 200, 200);

        gluPerspective(/* field of view in degree */ 40.0,
                /* aspect ratio */ 1.0,
                /* Z near */ 1.0, /* Z far */ 2 * arena->getRadius());

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        point3f p = currentBomb->getPosition();
        vector3f& v = currentBomb->getVelocity();

        // Look at its shadow. Shadow of the Bomb
        gluLookAt(p.x, p.y, p.z,
                p.x, p.y, 0,
                v.x, v.y, v.z);

        drawWorld();
    }

    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    drawHUD();

    // The minimap need to fit in 1/4 of the frame at the bottom-right position.

    int w = 500 / 4;
    int h = 500 / 4;

    const float padding = 10;

    glViewport(500 - w - padding, yOffset + padding, w, h);

    float r = arena->getRadius();

    // Make the frame fit into NDC
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-r, r, r, -r);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    drawMap();

    glEnable(GL_DEPTH_TEST);

    glutSwapBuffers();
}

void Game::drawWorld() {
    player->draw(
            cameraView == Camera::COCKPIT,
            cameraView == Camera::CANNON_VIEW ||
            cameraView == Camera::THIRD_PERSON_CAMERA,
            cameraView == Camera::CANNON_VIEW ||
            cameraView == Camera::THIRD_PERSON_CAMERA,
            cameraView != Camera::CANNON_VIEW
            );

    arena->draw();
    for (enemy_base_t* base : bases) {
        base->transformAndDraw();
    }
    for (flying_enemy_t* enemy : enemies) {
        enemy->transformAndDraw();
    }
    manager->draw();
}

void Game::drawHUD() {
    // Make the frame fit into NDC
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, height, 0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();
    {
        int w, h;

        measureText(scoreText.c_str(), &w, &h);

        const float padding = 10.0f;

        // center it at screen:
        glTranslatef(width - w - padding, padding + h, 0);
        // text with white color:
        glColor3f(1, 1, 1);

        drawText(scoreText.c_str());
    }
    glPopMatrix();

    if (player->getBehaviour() == Behaviour::GAME_OVER) {
        const char* text;
        if (player->isDead()) {
            // He lost!
            // TODO Put a sercastic message for the player instead
            text = "Perdeu... :(";
        } else {
            // He won!
            text = "Ganhou! :D";
        }

        int w, h;

        measureText(text, &w, &h);

        // center it at screen:
        glTranslatef((width - w) / 2, (height - h) / 2, 0);
        // text with white color:
        glColor3f(1, 1, 1);

        drawText(text);
    }
}

void Game::drawMap() {
    arena->drawMapElement(miniMapAux);

    // draw all of the ground enemies in orange (#FFA500)
    glColor3f(0xFF / 255.0, 0xA5 / 255.0, 0x00 / 255.0);
    for (enemy_base_t* e : bases) {
        e->drawMapElement(miniMapAux);
    }

    // Flying enemies have a red color
    glColor3f(1, 0, 0);
    for (flying_enemy_t* e : enemies) {
        e->drawMapElement(miniMapAux);
    }

    player->drawMapElement(miniMapAux);
}

void Game::mouseDragged(int x, int y) {
    if (followerOrbitEnabled) {
        follower->mouseDragged(x, y);
        glutPostRedisplay();
    }
}

void Game::mouseMoved(int x, int y) {
    if (player->getBehaviour() == Behaviour::CONTROLLING) {
        player->setCannonAxis(x / (GLfloat) width, y / (GLfloat) height);
    }
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
        followerOrbitEnabled = true;
    } else {
        if (button == GLUT_RIGHT_BUTTON && followerOrbitEnabled) {
            return;
        }
        player->mousePress(button);
    }
}

void Game::mouseReleased(int button, int x, int y) {
    followerOrbitEnabled = false;
}

void Game::reshape(int width, int height) {
    this->width = width;
    this->height = height;

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

    if (currentBomb != NULL && currentBomb->isDead()) {
        currentBomb = NULL;

        glutReshapeWindow(500, 700);
    }

    manager->collectGarbage();

    for (flying_enemy_t* enemy : enemies) {
        enemy->update(time);
        enemy->clipZ(arena->getHeight());
    }

    timeSinceLastEnemyFire += time;

    if (timeSinceLastEnemyFire > timeToEnemyFire) {
        timeSinceLastEnemyFire = 0;

        // All enemies are firing now!
        for (flying_enemy_t* e : enemies) {
            if (e->isAlive()) {
                point3f p = e->getPosition() + e->getCannonExit();
                vector3f v = e->getVelocity();

                bullet_t* b = new bullet_t(p, v, true);
                b->setRadius(e->getRadius() / 16);
                manager->addProjectile(b);
            }
        }
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
            // follower->setCamera(follower->getCamera() + (player->getPosition() - p));
        }
    }

    sAccumulatedTime += time;

    if (sAccumulatedTime * 30 > 1000) {
        glutPostRedisplay();
        sAccumulatedTime = 0;
    }

    watch->setMarkAsOffset();
}

void Game::keyPressed(unsigned char key, int x, int y) {
    switch (key) {
        case ' ':
            followerOrbitEnabled = true;
            break;
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
    followerOrbitEnabled = false;
}

void Game::reset() {
    printf("Resetting game\n");
    printf("Resetting listeners");
    for (reset_listener_t* l : sResetListeners) {
        l->reset();
    }
}

void Game::addResetListener(reset_listener_t * l) {
    sResetListeners.push_back(l);
}

void Game::onBaseDeath() {
    score++;
    scoreText = std::to_string(score);

    if (score >= bases.size()) {
        player->won();
    }
}

void Game::onBombThrow(bomb_t * b) {
    currentBomb = b;
    glutReshapeWindow(500, 700);
}