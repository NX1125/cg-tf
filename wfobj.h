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
#include "filepath.h"

using namespace std;

typedef void (*WFFunction)(const GLfloat*);

struct WFCommand {
    WFFunction apply;
    const char* name;
};

class wf_object_loader_t;

struct wf_material_t {
private:
    friend wf_object_loader_t;

    string name;

    WFCommand commands[5];
    float arguments[5 * 4];
    int n = 0;
};

class wf_object_t;

class wf_command_t {
public:

    virtual void apply(wf_object_t& obj) = 0;
};

/**
 * An object model that is parsed from a .obj file.
 */
class wf_object_t {
private:
    WFCommand* commands;
    float* arguments;

    int n;

    wf_object_t(WFCommand* commands, float* arguments, int n) :
    commands(commands), arguments(arguments), n(n) {
    }

public:

    ~wf_object_t() {
        free(commands);
        free(arguments);
    }

    void draw();

private:
    // commands
    friend wf_object_loader_t;

    static void ambient(const GLfloat* coords);

    static void diffuse(const GLfloat* coords);

    static void specular(const GLfloat* coords);

    static void shininess(const GLfloat* coords);

    static void begin(const GLfloat* ignore);

    static void end(const GLfloat* ignore);
};

/**
 * A class that loads an object per time. 
 */
class wf_object_loader_t {
private:
    map<string, wf_material_t> materials;

    vector<float> vertices;
    vector<float> normals;
    vector<float> textures;

    vector<WFCommand> commands;
    vector<float> arguments;

    wf_material_t currentMaterial;

    file_path* filepath = NULL;

public:
    wf_object_t* load(const char* filename);

    void loadResOnly(const char* filename);

    wf_object_t* loadRes(const char* name);

private:

    wf_object_t* load();

    void loadOnly();

    void loadMaterial(const char* filename);

    void loadMTL(char*);

    void loadOBJ(char*);

    void useMaterial(const char*);

    void put(WFFunction c, const char* name, vector<float>& v, int index);

    void put(WFFunction c, const char* name);

    void parseFace(char* line);

    static void loadMTL(char* line, wf_object_loader_t& obj) {
        obj.loadMTL(line);
    }

    static void loadOBJ(char* line, wf_object_loader_t& l) {
        l.loadOBJ(line);
    }

    void forEachLine(const char* filename, void (*)(char*, wf_object_loader_t&));

public:
    vector3f getMostDistantVertex() const;

    void scale(float s);

    wf_object_t* build();
};

#endif /* WFOBJ_H */

