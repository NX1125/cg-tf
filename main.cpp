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

/*
 * 
 */
int main(int argc, char** argv) {
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

        Game::init(settings);

        glutReshapeFunc(Game::reshape);
        glutDisplayFunc(Game::display);

        glutKeyboardFunc(Game::keyPress);
        // glutKeyboardUpFunc(keyUp);

        glutPassiveMotionFunc(Game::mouseMoved);
        glutMotionFunc(Game::mouseDragged);
        glutMouseFunc(Game::mouseButtonEvent);

        glutIdleFunc(Game::idle);

        // As stated in the spec, the initial frame size is 500x500
        const int initialWidth = 500;
        const int initialHeight = 500;
        
        glutInitWindowSize(initialWidth, initialHeight);
        glutReshapeWindow(initialWidth, initialHeight);
        // glutInitWindowPosition(100, 20);

        printf("Game context created\n");

        delete settings;

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

