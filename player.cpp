
#include <GL/freeglut_std.h>

#include "player.h"
#include "behaviourupdate.h"
#include "bomb.h"
#include "bullet.h"
#include "cube.h"

wf_object_t* player_t::sPlayerBodyModel = NULL;
float player_t::sPlayerBodyModelRadius = 0;

vector3f player_t::sBombDoor(0, 0, -1);

player_t::player_t(takeoff_t* takeoff, float radius) :
takeoff(takeoff), radius(radius) {
    position = takeoff->getStart();
    controller = new airplane_movement_t(position);
    cannon = new cannon_t(vector3f(radius * 0.8f, 0, -radius * 0.2f));

    float x = 1.0f * radius;
    float y = 0.6f * radius;
    float z = -0.19f * radius;
    
    propellerLeft = new propeller_t(vector3f(x, y, z));
    propellerRight = new propeller_t(vector3f(x, -y, z));

    float factor = 0.5f / radius;

    propellerLeft->setScaleFactor(factor);
    propellerRight->setScaleFactor(factor);

    cockpitOffset = vector3f(radius * 0.4f, 0, 0);

    horizontal = takeoff->getHorizontalAngle();
    vertical = takeoff->getVerticalAngle();
}

void player_t::setManager(projectile_manager_t* manager) {
    this->manager = manager;
    cannon->setManager(manager);
}

void player_t::sInit(wf_object_loader_t& loader) {
    sPlayerBodyModel = loader.loadRes("trenoSemHelice");
    sPlayerBodyModel->scale(1 / loader.getMostDistantVertex().length());
}

void player_t::cannonView() {
    vector3f off = cannon->getOffset();

    // off.x -= radius * 0.35f;

    off.rotateX(horizontalAngularVelocityDrawing);
    off.rotateY(vertical);
    off.rotateZ(horizontal);

    point3f p = position + off;

    point3f ep = position;
    vector3f v = cannon->getOffset() + cannon->getDirection() * cannon->getLength();

    v.rotateX(horizontalAngularVelocityDrawing);
    v.rotateY(vertical);
    v.rotateZ(horizontal);

    ep += v;

    float h = radius * 0.3f;

    gluLookAt(p.x, p.y, p.z + h,
            ep.x, ep.y, ep.z + h,
            0, 0, 1);
}

void player_t::cockpitView() {
    vector3f up(0, 0, 1);
    vector3f forward(1, 0, 0);

    up.rotateX(horizontalAngularVelocityDrawing);
    up.rotateY(vertical);
    up.rotateZ(horizontal);

    // forward = forward.rotateX(horizontalAngularVelocityDrawing);
    forward.rotateY(vertical);
    forward.rotateZ(horizontal);

    vector3f ck = cockpitOffset;

    ck.rotateX(horizontalAngularVelocityDrawing);
    ck.rotateY(vertical);
    ck.rotateZ(horizontal);

    point3f p = position + ck;

    gluLookAt(p.x, p.y, p.z,
            p.x + forward.x, p.y + forward.y, p.z + forward.z,
            up.x, up.y, up.z);
}

void player_t::keyPress(unsigned char key) {
    switch (mBehaviour) {
        case Behaviour::ON_GROUND:
            if (key == 'u' || key == 'U') {
                // lets start the takeoff
                printf("The airplane will take off!\n");
                mBehaviour = Behaviour::TAKING_OFF;
            }
            break;
        case Behaviour::TAKING_OFF:
            // reset is handled by the Game class.
            break;
        case Behaviour::CONTROLLING:
            // The user takes control of the airplane
            controller->keyPress(key);
            break;
        case Behaviour::GAME_OVER:
            break;
    }
}

void player_t::keyRelease(unsigned char key) {
    if (mBehaviour == Behaviour::CONTROLLING) {
        controller->keyReleased(key);
    }
}

void player_t::update(int millis) {
    // The player has four behaviours, but only two of them are needed in this
    // update.

    behaviour_update_t* behaviour = NULL;

    switch (mBehaviour) {
        case Behaviour::TAKING_OFF:
            behaviour = takeoff;
            break;
        case Behaviour::CONTROLLING:
            behaviour = controller;
            break;
        case Behaviour::ON_GROUND:
            // the horizontal is constant
            horizontal = takeoff->getHorizontalAngle();
            // the player is at the ground parallel to it
            vertical = 0;
            // There is no rotation around its own axis yet.
            horizontalAngularVelocity = 0;
            break;
    }
    float velocity = 0;

    if (behaviour != NULL) {
        behaviour->update(millis);
        position = behaviour->getPosition();

        velocity = behaviour->getVelocityMagnitude();

        // printf("(%f, %f, %f)\n", position.x, position.y, position.z);
        horizontal = behaviour->getHorizontalAngle();
        vertical = behaviour->getVerticalAngle();
        // There is no rotation around its own axis yet.

        const float angleFactor = 1.0f;

        horizontalAngularVelocity = behaviour->getHorizontalAngularVelocity() * angleFactor;
    }

    if (mBehaviour == Behaviour::TAKING_OFF && takeoff->isCompleted()) {
        // transition from takeoff to give the controller to the user.
        mBehaviour = Behaviour::CONTROLLING;

        controller->setPosition(position);
        controller->setInitialMagnitude(takeoff->getFinalVelocity() * velocityFactor);
        controller->setAngles(takeoff->getHorizontalAngle(), 0);
    }

    const float k = 0.9f;

    horizontalAngularVelocityDrawing *= k;
    horizontalAngularVelocityDrawing += horizontalAngularVelocity * (1 - k);

    propellerLeft->update(millis);
    propellerRight->update(millis);

    propellerLeft->setMagnitude(velocity);
    propellerRight->setMagnitude(velocity);
}

void player_t::draw(bool cockpit, bool gun, bool body, bool aim) {
    if (dead) {
        return;
    }
    // TODO Add beam of light from the player when it is night mode

    // glDisable(GL_CULL_FACE);
    glPushMatrix();
    {
        if (aim) {
            point3f p = position;

            vector3f v = cannon->getOffset();

            v.rotateX(horizontalAngularVelocityDrawing);
            v.rotateY(vertical);
            v.rotateZ(horizontal);

            //            glPushMatrix();
            //            {
            //                glTranslatef(p.x + v.x, p.y + v.y, p.z + v.z);
            //                glutSolidSphere(2, 8, 8);
            //            }
            //            glPopMatrix();

            v = cannon->getOffset() + cannon->getDirection() * cannon->getLength();

            v.rotateX(horizontalAngularVelocityDrawing);
            v.rotateY(vertical);
            v.rotateZ(horizontal);

            p += v;

            glPushMatrix();
            {
                glTranslatef(p.x, p.y, p.z);
                glutSolidSphere(2, 8, 8);
            }
            glPopMatrix();

            v = cannon->getDirection();

            v.rotateX(horizontalAngularVelocityDrawing);
            v.rotateY(vertical);
            v.rotateZ(horizontal);

            p += v * 5;

            glPushMatrix();
            {
                glTranslatef(p.x, p.y, p.z);
                glutSolidSphere(2, 8, 8);
            }
            glPopMatrix();
        }

        glTranslatef(position.x, position.y, position.z);

        drawAxis(radius);
        // considering that the front of the airplane is at +x and the back is at -x
        const float degreePerRadians = 180 / M_PI;
        glRotatef(horizontal * degreePerRadians, 0, 0, 1);
        glRotatef(vertical * degreePerRadians, 0, 1, 0);
        glRotatef(horizontalAngularVelocityDrawing * degreePerRadians, 1, 0, 0);
        if (gun) {
            cannon->draw();
        }
        // cockpit
        if (cockpit) {
            glPushMatrix();
            {
                glTranslatef(cockpitOffset.x, cockpitOffset.y, cockpitOffset.z);
                // drawBox();
            }
            glPopMatrix();
        }
        glPushMatrix();
        {
            glTranslatef(cockpitOffset.x, cockpitOffset.y, cockpitOffset.z);
            glutSolidSphere(1, 8, 8);
        }
        glPopMatrix();
        if (body) {
            glPushMatrix();
            {
                // glRotatef(90, 0, 1, 0);
                propellerLeft->transformAndDraw(radius);
                propellerRight->transformAndDraw(radius);
            }
            glPopMatrix();

            glRotatef(90, 1, 0, 0);
            glScalef(radius, radius, radius);
            // glutSolidSphere(1, 8, 8);
            drawAxis(radius);
            const float s = 1.3;
            glScalef(s, s, s);

            GLfloat color[] = {1, 1, 1, 1};

            glColor3fv(color);
            //            glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
            //             glMaterialfv(GL_FRONT, GL_SPECULAR, color);
            //             glMaterialf(GL_FRONT, GL_SHININESS, 50.0f);

            if (sPlayerBodyModel == NULL) {
                glutSolidSphere(2, 32, 32);
            } else {
                sPlayerBodyModel->draw();
            }
        }
    }
    glPopMatrix();
}

vector3f player_t::getVelocity() const {
    return controller->getVelocity();
}

vector3f player_t::getDirection(float vt) const {
    vector3f v(1, 0, 0);

    v.rotateY(vertical + vt);
    v.rotateZ(horizontal);

    return v;
}

void player_t::setPosition(const point3f& p) {
    position = p;
    controller->setPosition(p);
}

const char* player_t::getName() const {
    return "player";
}

void player_t::clipZ(float height) {
    if (mBehaviour != Behaviour::CONTROLLING) return;
    if (position.z < radius) {
        // printf("The player is hitting the ground!\n");
        position.z = radius;
        // kill();
    } else if (position.z > height - radius) {
        // printf("The player is hitting the roof!\n");
        position.z = height - radius;
    }
}

void player_t::bomb() {
    printf("The player just threw a bomb\n");
    vector3f v = controller->getVelocity();
    v.z = 0;
    bomb_t* b = new bomb_t(position + sBombDoor * radius, v, horizontal * 180 / M_PI);
    b->setRadius(radius / 8);
    manager->addProjectile(b);

    if (bombListener != NULL) {
        bombListener->onBombThrow(b);
    }
}

void player_t::fire() {
    point3f p = position;

    vector3f v = cannon->getOffset();

    v.rotateX(horizontalAngularVelocityDrawing);
    v.rotateY(vertical);
    v.rotateZ(horizontal);

    point3f p1 = p + v;

    v = cannon->getOffset() + cannon->getDirection() * cannon->getLength();

    v.rotateX(horizontalAngularVelocityDrawing);
    v.rotateY(vertical);
    v.rotateZ(horizontal);

    p += v;

    vector3f v2 = p - p1;

    v2.normalize();

    bullet_t* bullet = new bullet_t(p,
            v2 * (bulletVelocityFactor * controller->getMagnitude()), false);
    bullet->setRadius(radius / 16);
    manager->addProjectile(bullet);
}

void player_t::mousePress(int button) {
    if (mBehaviour == Behaviour::CONTROLLING) {
        switch (button) {
            case GLUT_RIGHT_BUTTON:
                bomb();
                break;
            case GLUT_LEFT_BUTTON:
                fire();
                break;
        }
    }
}

void player_t::kill() {
    if (!canDie() || dead) return;

    dead = true;

    mBehaviour = Behaviour::GAME_OVER;

    if (death != NULL) {
        death->onPlayerDeath();
    }
}

bool player_t::canDie() const {
    return mBehaviour == Behaviour::CONTROLLING;
}

void player_t::setCannonAxis(float x, float y) {
    cannon->setInputAxis(x, y);
}

void player_t::won() {
    mBehaviour = Behaviour::GAME_OVER;
}

void player_t::drawMapElement(circle_blueprint_t* blueprint) const {
    if (dead) return;
    // Make the player green
    glColor3f(0, 1, 0);
    glPushMatrix();
    {
        // Draw a circle at the player position
        glTranslatef(position.x, position.y, 0);
        blueprint->draw(true, radius);
    }
    glPopMatrix();
}
