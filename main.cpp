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

#define TF_VERSION "0.1.2"

void loadSettings(char* configFile, app_settings& settings) {
    file_path path(configFile);

    path.append("config.xml");

    printf("Trying to open %s\n", path.toString());

    settings = app_settings(path.toString());
}

/*
 * 
 */
int main(int argc, char** argv) {
    glutInit(&argc, argv);

    app_settings settings;

    try {
        if (argc < 2) {
            printf("syntax: %s <config folder>\n", argv[0]);
            return EXIT_SUCCESS;
        }
        
        loadSettings(argv[1], settings);

        // TODO create game context (objects, GLUT, etc)

        printf("Game context created\n");
    } catch (IOException& ex) {
        printf("[error] An exception: %s\n", ex.what());
    } catch (MissingElementException& ex) {
        printf("[error] An exception: %s\n", ex.what());
    } catch (AttributeException& ex) {
        printf("[error] An exception: %s\n", ex.what());
    }

    return EXIT_SUCCESS;
}

