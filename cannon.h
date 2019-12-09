
/* 
 * File:   cannon.h
 * Author: 2019202344
 *
 * Created on 6 de Dezembro de 2019, 19:54
 */

#ifndef CANNON_H
#define CANNON_H

#include "shapes.h"
#include "wfobj.h"
#include "projectile.h"

class cannon_t {
private:

    static wf_object_t* sModel;

    /**
     * Position relative to the airplane.
     */
    vector3f offset;

    float horizontal = 0;
    float vertical = 0;

    float horizontalFactor = 0.3f;
    float verticalFactor = 0.3f;

    projectile_manager_t* manager = NULL;

    float length = 10;
    float width = 5;

public:

    cannon_t(const vector3f& position);

    void setManager(projectile_manager_t* manager) {
        this->manager = manager;
    }

    static void init(wf_object_loader_t& loader);

    void draw();

    void setInputAxis(float x, float y);

    vector3f getDirection() const;

    vector3f getDirection(float h, float v) const;

    float getLength() const {
        return length;
    }

    vector3f getOffset() const {
        return offset;
    }

    float getWidth() const {
        return width;
    }

    float getAim() const {
        return width;
    }

    static void draw0() {
        glColor3f(1, 1, 1);
        glPushMatrix();
        {
            glScalef(10, 10, 10);
            sModel->draw();
        }
        glPopMatrix();
    }

};

#endif /* CANNON_H */

