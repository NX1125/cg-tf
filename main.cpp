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
        vertices[0][0] = vertices[1][0] = vertices[2][0] = vertices[3][0] = -1;
        vertices[4][0] = vertices[5][0] = vertices[6][0] = vertices[7][0] = 1;
        vertices[0][1] = vertices[1][1] = vertices[4][1] = vertices[5][1] = -1;
        vertices[2][1] = vertices[3][1] = vertices[6][1] = vertices[7][1] = 1;
        vertices[0][2] = vertices[3][2] = vertices[4][2] = vertices[7][2] = 1;
        vertices[1][2] = vertices[2][2] = vertices[5][2] = vertices[6][2] = -1;
        
        glEnable(GL_DEPTH_TEST);
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

        glLoadIdentity();
        glOrtho(-1, 1, -1, 1, 1, -1);

        // Draw a cube for reference to the camera
        // glColor3f(1, 0, 0);
        glScaled(0.5f, 0.5f, 0.5f);
        // glRotated(180 * stopwatch_t::currentTimeMillis() / 1000.0f, 0, 1, 1);
        drawBox();

        glutSwapBuffers();
    }

    static void reshape(int width, int height) {
        sWidth = width;
        sHeight = height;

        glViewport(0, 0, width, height);

        printf("Reshape: (%d, %d)\n", width, height);
    }

    static void idle() {
        glutPostRedisplay();
    }
};


GLfloat Main::light_diffuse[] = {1.0, 0.0, 0.0, 1.0}; /* Red diffuse light. */
GLfloat Main::light_position[] = {1.0, 1.0, 1.0, 0.0}; /* Infinite light location. */
GLfloat Main::normals[6][3] = {/* Normals for the 6 faces of a cube. */
    {-1.0, 0.0, 0.0},
    {0.0, 1.0, 0.0},
    {1.0, 0.0, 0.0},
    {0.0, -1.0, 0.0},
    {0.0, 0.0, 1.0},
    {0.0, 0.0, -1.0}
};
GLint Main::faces[6][4] = {/* Vertex indices for the 6 faces of a cube. */
    {0, 1, 2, 3},
    {3, 2, 6, 7},
    {7, 6, 5, 4},
    {4, 5, 1, 0},
    {5, 6, 2, 1},
    {7, 4, 0, 3}
};
GLfloat Main::vertices[8][3]; /* Will be filled in with X,Y,Z vertexes. */
GLfloat Main::color[6][3] = {
    {1,0,0},
    {0,1,0},
    {0,0,1},
    {1,1,0},
    {0,1,1},
    {1,0,1},
};


// As stated in spec, the size of the window is initially 500x500.
int Main::sWidth = 500;
int Main::sHeight = 500;

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

        Main::init();

        // TODO create game context (objects, GLUT, etc)
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

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

