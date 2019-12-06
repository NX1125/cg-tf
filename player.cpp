
#include "player.h"
#include "behaviourupdate.h"

wf_object_t* player_t::sPlayerBodyModel = NULL;

player_t::player_t(takeoff_t* takeoff, float radius) :
takeoff(takeoff), radius(radius) {
    position = takeoff->getStart();
    controller = new airplane_movement_t();
}

void player_t::sInit(wf_object_loader_t& loader) {
    sPlayerBodyModel = loader.loadRes("trenoSemHelice");
}

void player_t::draw() {
    glPushMatrix();
    {
        glTranslatef(position.x, position.y, position.z);
        drawAxis(radius);
        // considering that the front of the airplane is at +y and the back is at -y
        glRotatef(horizontal, 0, 0, 1);
        glRotatef(vertical, 1, 0, 0);
        glRotatef(horizontalAngularVelocity, 0, 1, 0);
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
    if (behaviour != NULL) {
        behaviour->update(millis);
        position = behaviour->getPosition();
        // printf("(%f, %f, %f)\n", position.x, position.y, position.z);
        horizontal = behaviour->getHorizontalAngle();
        vertical = behaviour->getVerticalAngle();
        // There is no rotation around its own axis yet.
        horizontalAngularVelocity = behaviour->getHorizontalAngularVelocity();
    }

    if (mBehaviour == Behaviour::TAKING_OFF && takeoff->isCompleted()) {
        // transition from takeoff to give the controller to the user.
        mBehaviour = Behaviour::CONTROLLING;
        controller->setPosition(position);
        controller->setMagnitude(takeoff->getFinalVelocity() * velocityFactor);
        controller->setAngles(takeoff->getHorizontalAngle(), 0);
    }
}

vector3f player_t::getVelocity() const {
    return controller->getVelocity();
}

void player_t::setPosition(const point3f& p) {
    controller->setPosition(p);
}

const char* player_t::getName() const {
    return "player";
}

