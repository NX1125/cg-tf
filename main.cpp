/* 
 * File:   main.cpp
 * Author: gg
 *
 * Created on 2 de outubro de 2019, 19:26
 */

#include <cstdlib>
#include <GL/freeglut_std.h>

#include "filepath.h"
#include "settings.h"
#include "xmlutils.h"
#include "simplesvg.h"
#include "stopwatch.h"
#include "thirdpersonfollower.h"
#include "game.h"

using namespace std;

app_settings* loadSettings(char* configFile) {
    file_path path(configFile);

    path.append("config.xml");

    printf("Trying to open %s\n", path.toString());

    return new app_settings(path.toString());
}

Game* sGame = NULL;

void display();

void mouseDragged(int x, int y);

void mouseMoved(int x, int y);

void mouseButtonEvent(int button, int state, int x, int y);

void mousePressed(int button, int x, int y);

void mouseReleased(int button, int x, int y);

void reshape(int width, int height);

void idle();

void keyPressed(unsigned char key, int x, int y);

void keyReleased(unsigned char key, int x, int y);

/*
 * 
 */
int main(int argc, char** argv) {
    printf("TODO Create embedded presentation\n");
    
    glutInit(&argc, argv);

    try {
        if (argc < 2) {
            printf("syntax: %s <config folder>\n", argv[0]);
            return EXIT_SUCCESS;
        }

        app_settings* settings = loadSettings(argv[1]);

        // TODO create game context (objects, GLUT, etc)
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

        std::string title("TF - Guilherme, Ricardo");

        glutCreateWindow(title.c_str());

        sGame = new Game(settings);

        glutReshapeFunc(reshape);
        glutDisplayFunc(display);

        glutKeyboardFunc(keyPressed);
        glutKeyboardUpFunc(keyReleased);

        glutPassiveMotionFunc(mouseMoved);
        glutMotionFunc(mouseDragged);
        glutMouseFunc(mouseButtonEvent);

        glutIdleFunc(idle);

        // As stated in the spec, the initial frame size is 500x500
        const int initialWidth = 500;
        const int initialHeight = 500;

        glutInitWindowSize(initialWidth, initialHeight);
        glutReshapeWindow(initialWidth, initialHeight);
        // glutInitWindowPosition(100, 20);

        printf("Game context created\n");

        delete settings;

        glutMainLoop();
        
        delete sGame;
    } catch (IOException& ex) {
        printf("[error] An exception: %s\n", ex.what());
    } catch (MissingElementException& ex) {
        printf("[error] An exception: %s\n", ex.what());
    } catch (AttributeException& ex) {
        printf("[error] An exception: %s\n", ex.what());
    }

    return EXIT_SUCCESS;
}

// wrappers for GLUT to Game

void display() {
    sGame->display();
}

void mouseDragged(int x, int y) {
    sGame->mouseDragged(x, y);
}

void mouseMoved(int x, int y) {
    sGame->mouseMoved(x, y);
}

void mouseButtonEvent(int button, int state, int x, int y) {
    sGame->mouseButtonEvent(button, state, x, y);
}

void mousePressed(int button, int x, int y) {
    sGame->mousePressed(button, x, y);
}

void mouseReleased(int button, int x, int y) {
    sGame->mouseReleased(button, x, y);
}

void reshape(int width, int height) {
    sGame->reshape(width, height);
}

void idle() {
    sGame->idle();
}

void keyPressed(unsigned char key, int x, int y) {
    sGame->keyPressed(key, x, y);
}

void keyReleased(unsigned char key, int x, int y) {
    sGame->keyReleased(key, x, y);
}
