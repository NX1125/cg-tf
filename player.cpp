
#include <GL/freeglut_std.h>

#include "player.h"
#include "behaviourupdate.h"
#include "bomb.h"

wf_object_t* player_t::sPlayerBodyModel = NULL;

vector3f player_t::sBombDoor(0, 0, -1);

player_t::player_t(takeoff_t* takeoff, float radius) :
takeoff(takeoff), radius(radius) {
    position = takeoff->getStart();
    controller = new airplane_movement_t();
    cannon = new cannon_t(vector3f(radius, 0, 0));

    propellerLeft = new propeller_t(vector3f(0, radius, radius));
    propellerRight = new propeller_t(vector3f(0, -radius, radius));
    
    float factor = 0.5f / radius;
    
    propellerLeft->setScaleFactor(factor);
    propellerRight->setScaleFactor(factor);
}

void player_t::setManager(projectile_manager_t* manager) {
    this->manager = manager;
    cannon->setManager(manager);
}

void player_t::sInit(wf_object_loader_t& loader) {
    sPlayerBodyModel = loader.loadRes("trenoSemHelice");
}

void player_t::draw() {
    glPushMatrix();
    {
        glTranslatef(position.x, position.y, position.z);
        drawAxis(radius);
        // considering that the front of the airplane is at +x and the back is at -x
        const float degreePerRadians = 180 / M_PI;
        glRotatef(horizontal * degreePerRadians, 0, 0, 1);
        glRotatef(vertical * degreePerRadians, 0, 1, 0);
        glRotatef(horizontalAngularVelocityDrawing * degreePerRadians, 1, 0, 0);
        cannon->draw();
        propellerLeft->draw();
        propellerRight->draw();
        glRotatef(90, 1, 0, 0);
        glScalef(radius, radius, radius);
        drawAxis(radius);
        // glScalef(100, 100, 100);

        sPlayerBodyModel->draw();
    }
    glPopMatrix();
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

        const float angleFactor = 0.02f;

        horizontalAngularVelocity = behaviour->getHorizontalAngularVelocity() * angleFactor;
    }

    if (mBehaviour == Behaviour::TAKING_OFF && takeoff->isCompleted()) {
        // transition from takeoff to give the controller to the user.
        mBehaviour = Behaviour::CONTROLLING;
        
        controller->setPosition(position);
        controller->setMagnitude(takeoff->getFinalVelocity() * velocityFactor);
        controller->setAngles(takeoff->getHorizontalAngle(), 0);
    }

    const float k = 0.8f;

    horizontalAngularVelocityDrawing *= k;
    horizontalAngularVelocityDrawing += horizontalAngularVelocity * (1 - k);
    
    propellerLeft->update(millis);
    propellerRight->update(millis);
    
    propellerLeft->setMagnitude(velocity);
    propellerRight->setMagnitude(velocity);
}

vector3f player_t::getVelocity() const {
    return controller->getVelocity();
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
    } else if (position.z > height - radius) {
        // printf("The player is hitting the roof!\n");
        position.z = height - radius;
    }
}

void player_t::bomb() {
    printf("The player just threw a bomb\n");
    // TODO Create a bomb and attach it to projectiles
    vector3f v = controller->getVelocity();
    v.z = 0;
    bomb_t* b = new bomb_t(position + sBombDoor * radius, v);
    b->setRadius(radius / 8);
    manager->addProjectile(b);
}

void player_t::fire() {
    printf("The player just fired\n");
    // TODO Create a bullet and attach it to projectiles
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
    dead = true;
}

bool player_t::canDie() const {
    return mBehaviour == Behaviour::CONTROLLING;
}

void player_t::setCannonAxis(float x, float y) {
    cannon->setInputAxis(x, y);
}

