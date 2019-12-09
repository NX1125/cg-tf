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

struct WFCommand;

typedef void (*WFFunction)(const GLfloat*, WFCommand*);

struct WFCommand {
    WFFunction apply;
    const char* name;
    
    GLuint texture;
};

class wf_object_loader_t;

#define MATERIAL_COMMANDS_COUNT 6

struct wf_material_t {
private:
    friend wf_object_loader_t;

    string name;

    WFCommand commands[MATERIAL_COMMANDS_COUNT];
    float arguments[MATERIAL_COMMANDS_COUNT * 4];
    int n = 0;

    string* texture = NULL;

public:

    wf_material_t() {
    }

    ~wf_material_t() {
    }
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
    
    float s = 1;

    wf_object_t(WFCommand* commands, float* arguments, int n) :
    commands(commands), arguments(arguments), n(n) {
    }

public:

    ~wf_object_t() {
        delete (commands);
        free(arguments);
    }

    void draw();
    
    void scale(float s1) {
        s = s1;
    }

private:
    // commands
    friend wf_object_loader_t;

    static void ambient(const GLfloat* coords, WFCommand* data);

    static void diffuse(const GLfloat* coords, WFCommand* data);

    static void specular(const GLfloat* coords, WFCommand* data);

    static void shininess(const GLfloat* coords, WFCommand* data);

    static void vertex(const GLfloat* coords, WFCommand* data);

    static void normal(const GLfloat* coords, WFCommand* data);

    static void tex(const GLfloat* coords, WFCommand* data);

    static void begin(const GLfloat* ignore, WFCommand* data);

    static void end(const GLfloat* ignore, WFCommand* data);
    
    static void bindTexture(const GLfloat* ignore, WFCommand* data);
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

    vector<string*> garbage;
    
    float mScale = 1;

public:
    wf_object_t* load(const char* filename);

    void loadResOnly(const char* filename);

    wf_object_t* loadRes(const char* name);

    ~wf_object_loader_t() {
    }

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

    void normalize();

    wf_object_t* build();
};

#endif /* WFOBJ_H */

