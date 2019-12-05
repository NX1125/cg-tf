/* 
 * File:   wfobj.h
 * Author: gg
 *
 * Created on 4 de dezembro de 2019, 21:45
 */

#ifndef WFOBJ_H
#define WFOBJ_H

#include <map>
#include <vector>

#include "xmlutils.h"
#include "shapes.h"

using namespace std;

typedef void (*WFCommand)(const GLfloat*);

class WFObjectLoader;

/**
 * An object model that is parsed from a .obj file.
 */
class WFObject {
private:
    WFCommand* commands;
    float* arguments;

    int n;

    WFObject(WFCommand* commands, float* arguments, int n) :
    commands(commands), arguments(arguments), n(n) {
    }

public:

    ~WFObject() {
        delete commands;
        delete arguments;
    }

    void draw();

private:
    // commands
    friend WFObjectLoader;

    static void ambient(const GLfloat* coords) {
        //        glMaterialfv(GL_FRONT, GL_AMBIENT, coords);
    }

    static void diffuse(const GLfloat* coords) {
        //        glMaterialfv(GL_FRONT, GL_DIFFUSE, coords);
    }

    static void specular(const GLfloat* coords) {
        //        glMaterialfv(GL_FRONT, GL_SPECULAR, coords);
    }

    static void shininess(const GLfloat* coords) {
        //        glMaterialfv(GL_FRONT, GL_SHININESS, coords);
    }

    static void begin(const GLfloat* ignore) {
        glBegin(GL_POLYGON);
    }

    static void end(const GLfloat* ignore) {
        glEnd();
    }
};

struct WFMaterial {
private:
    friend WFObjectLoader;

    string name;

    WFCommand commands[5];
    float arguments[5 * 4];
    int n;
};

/**
 * A class that loads an object per time. 
 */
class WFObjectLoader {
private:
    map<string, WFMaterial> materials;

    vector<float> vertices;
    vector<float> normals;
    vector<float> textures;

    vector<WFCommand> commands;
    vector<float> arguments;

    WFMaterial currentMaterial;

public:
    WFObject* load(const char* objFilename);

private:

    void loadMaterial(const char* mtlFilename);

    void loadMTL(char*);

    void loadOBJ(char*);

    void useMaterial(const char*);

    void put(WFCommand c, vector<float>& v, int index);

    void put(WFCommand c);

    static void loadMTL(char* line, WFObjectLoader& obj) {
        obj.loadMTL(line);
    }

    static void loadOBJ(char* line, WFObjectLoader& l) {
        l.loadOBJ(line);
    }

    void forEachLine(const char* filename, void (*)(char*, WFObjectLoader&));

    WFObject* build();
};

#endif /* WFOBJ_H */

