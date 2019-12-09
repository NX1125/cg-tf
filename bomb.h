
/* 
 * File:   bomb.h
 * Author: 2019202344
 *
 * Created on 6 de Dezembro de 2019, 15:50
 */

#ifndef BOMB_H
#define BOMB_H

#include "projectile.h"
#include "wfobj.h"

class bomb_t : public projectile_t {
private:

    static wf_object_t* sBombModel;

    static float sGravityAcceleration;

    float radius = 1;

    float horizontal = 0;
public:

    bomb_t(const point3f& offset, const vector3f& velocity, float h);

    void setRadius(float radius) {
        this->radius = radius;
    }

    void move(float time) override;

    void hit(obstacle_t* other) override;

    static void setGravityAcceleration(float acc);

    void draw() const override;

    float getRadius() const override {
        // TODO Measure bomb radius
        return radius;
    }

    static void draw0() {
        glColor3f(1,1,1);
        glPushMatrix();
        {
            glScalef(10, 10, 10);
            sBombModel->draw();
        }
        glPopMatrix();
    }

public:

    static void init(wf_object_loader_t& loader);
};

#endif /* BOMB_H */

