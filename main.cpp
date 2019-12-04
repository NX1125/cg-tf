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

using namespace std;

app_settings* loadSettings(char* configFile) {
    file_path path(configFile);

    path.append("config.xml");

    printf("Trying to open %s\n", path.toString());

    return new app_settings(path.toString());
}

/**
 * A static class, just to not use global variables, as asked in a previous TC.
 */
class Main {
public:

    static int sWidth;
    static int sHeight;

    static void init() {
        glClearColor(0, 0, 0, 0);
    }

    static void display() {
        glClear(GL_COLOR_BUFFER_BIT);
        
        glutSwapBuffers();
    }

    static void reshape(int width, int height) {
        sWidth = width;
        sHeight = height;

        glViewport(0, 0, width, height);

        printf("Reshape: (%d, %d)\n", width, height);
    }

    static void idle() {
    }
};

// As stated in spec, the size of the window is initially 500x500.
int Main::sWidth = 500;
int Main::sHeight = 500;

/*
 * 
 */
int main(int argc, char** argv) {
    printf("myself: %s\n", argv[0]);

    glutInit(&argc, argv);

    try {
        if (argc < 2) {
            printf("syntax: %s <config folder>\n", argv[0]);
            return EXIT_SUCCESS;
        }

        app_settings* settings = loadSettings(argv[1]);

        Main::init();

        // TODO create game context (objects, GLUT, etc)
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

        std::string title("TF - Guilherme, Ricardo");

        glutCreateWindow(title.c_str());

        glutReshapeFunc(Main::reshape);
        glutDisplayFunc(Main::display);

        // glutKeyboardFunc(keyDown);
        // glutKeyboardUpFunc(keyUp);

        // glutMouseFunc(mouseButton);
        // glutPassiveMotionFunc(passiveMotion);

        glutIdleFunc(Main::idle);

        glutInitWindowSize(Main::sWidth, Main::sHeight);
        glutReshapeWindow(Main::sWidth, Main::sHeight);
        // glutInitWindowPosition(100, 20);

        printf("Game context created\n");

        glutMainLoop();
    } catch (IOException& ex) {
        printf("[error] An exception: %s\n", ex.what());
    } catch (MissingElementException& ex) {
        printf("[error] An exception: %s\n", ex.what());
    } catch (AttributeException& ex) {
        printf("[error] An exception: %s\n", ex.what());
    }

    return EXIT_SUCCESS;
}

