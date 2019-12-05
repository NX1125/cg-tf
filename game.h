/* 
 * File:   game.h
 * Author: gg
 *
 * Created on 5 de dezembro de 2019, 01:34
 */

#ifndef GAME_H
#define GAME_H

#include <GL/glut.h>

#include "shapes.h"
#include "thirdpersonfollower.h"
#include "settings.h"
#include "resetlistener.h"
#include "wfobj.h"

enum Behaviour {
    ON_GROUND, TAKING_OFF, CONTROLLING, GAME_OVER
};

/**
 * A static class, just to not use global variables, as asked in a previous TC.
 */
class Game {
public:

    static int sWidth;
    static int sHeight;

    static third_person_follower_t* sFollower;
    static point3f sTarget;

    static bool sFollowerMouseEnabled;

    static Behaviour sBehaviour;

    static vector<reset_listener_t*> sResetListeners;
    
    static WFObject* sHouseModel;

    static void init(app_settings* settings) {
        glClearColor(0, 0, 0, 0);
        vertices[0][0] = vertices[1][0] = vertices[2][0] = vertices[3][0] = -1;
        vertices[4][0] = vertices[5][0] = vertices[6][0] = vertices[7][0] = 1;
        vertices[0][1] = vertices[1][1] = vertices[4][1] = vertices[5][1] = -1;
        vertices[2][1] = vertices[3][1] = vertices[6][1] = vertices[7][1] = 1;
        vertices[0][2] = vertices[3][2] = vertices[4][2] = vertices[7][2] = 1;
        vertices[1][2] = vertices[2][2] = vertices[5][2] = vertices[6][2] = -1;

        glEnable(GL_DEPTH_TEST);

        sFollower = new third_person_follower_t(&sTarget, 0.5f);

        loadModels();
    }

    static void loadModels() {
        WFObjectLoader loader;

        printf("Loading house model from \"casa.obj\"\n");
        sHouseModel = loader.load("./models/casa.obj");
    }

    // Extracted from:
    // https://www.opengl.org/archives/resources/code/samples/glut_examples/examples/cube.c
    // Will delete after the third-person-camera is done
    static GLfloat light_diffuse[];
    static GLfloat light_position[];
    static GLfloat normals[6][3];
    static GLint faces[6][4];
    static GLfloat vertices[8][3]; /* Will be filled in with X,Y,Z vertexes. */
    static GLfloat color[6][3];

    static void drawBox(void) {
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

    static void display() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        sFollower->lookAt();

        glOrtho(-2, 2, -2, 2, 2, -2);

        // Draw a cube for reference to the camera
        // glColor3f(1, 0, 0);
        glScaled(0.5f, 0.5f, 0.5f);
        // glRotated(180 * stopwatch_t::currentTimeMillis() / 1000.0f, 0, 1, 1);
        drawBox();

        glutSwapBuffers();
    }

    static void mouseDragged(int x, int y) {
        if (sFollowerMouseEnabled) {
            sFollower->mouseDragged(x, y);
            glutPostRedisplay();
        }
    }

    static void mouseMoved(int x, int y) {
        // TODO Move cannon of player
    }

    static void mouseButtonEvent(int button, int state, int x, int y) {
        switch (state) {
            case GLUT_DOWN:
                mousePressed(button, x, y);
                break;
            case GLUT_UP:
                mouseReleased(button, x, y);
                break;
        }
    }

    static void mousePressed(int button, int x, int y) {
        if (button == GLUT_RIGHT_BUTTON && sBehaviour == Behaviour::ON_GROUND) {
            sFollower->setMousePressingPosition(x, y);
            sFollowerMouseEnabled = true;
        }
    }

    static void mouseReleased(int button, int x, int y) {
        sFollowerMouseEnabled = false;
    }

    static void reshape(int width, int height) {
        sWidth = width;
        sHeight = height;

        glViewport(0, 0, width, height);

        printf("Reshape: (%d, %d)\n", width, height);
    }

    static void idle() {
    }

    static void keyPress(unsigned char key, int x, int y) {
        switch (key) {
            case 'r':
                reset();
                break;
            case 'u':
                break;
            default:
                break;
        }
    }

    static void reset() {
        for (reset_listener_t* l : sResetListeners) {
            l->reset();
        }
    }

    static void addResetListener(reset_listener_t* l) {
        sResetListeners.push_back(l);
    }
};


#endif /* GAME_H */

