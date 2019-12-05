
#include "wfobj.h"

void WFObject::draw() {
    float* k = arguments;
    for (int i = 0; i < n; i++, k += 4) {
        // printf("c(%f, %f, %f, %f)\n", k[0], k[1], k[2], k[3]);
        commands[i](k);
    }
}

WFObject* WFObjectLoader::build() {
    WFCommand* cs = (WFCommand*) malloc(commands.size() * sizeof(WFCommand));
    float* args = (float*) malloc(arguments.size() * sizeof(float));
    
    for (int i = 0; i < commands.size(); i++) {
        cs[i] = commands[i];
    }
    for (int i = 0; i < arguments.size(); i++) {
        args[i] = arguments[i];
    }
    
    printf("%ld commands loaded\n", commands.size());
    
    return new WFObject(cs, args, commands.size());
}

WFObject* WFObjectLoader::load(const char* objFilename) {
    forEachLine(objFilename, loadOBJ);
    
    return build();
}

void WFObjectLoader::loadMaterial(const char* mtlFilename) {
    forEachLine(mtlFilename, loadMTL);
    materials[currentMaterial.name] = currentMaterial;
}

void WFObjectLoader::loadMTL(char* line) {
    // newmtl <name>
    // Ka <r> <g> <b>
    // Kd <r> <g> <b>
    // KS <r> <g> <b>
    // Ns <range>

    char name[20];
    sscanf(line, "%s", name);

    float args[4];
    WFCommand c;

    switch (name[0]) {
        case 'K':
            sscanf(line, "%*s %f%f%f", args, args + 1, args + 2);
            args[3] = 1.0f;
            switch (name[1]) {
                case 'a':
                    c = WFObject::ambient;
                    break;
                case 'd':
                    c = WFObject::diffuse;
                    break;
                case 's':
                    c = WFObject::specular;
                    break;
            }
            break;
        case 'N':
            sscanf(line, "Ns %f", args);
            args[1] = 0;
            args[2] = 0;
            args[3] = 0;
            c = WFObject::shininess;
            break;
        case 'n':
            materials[currentMaterial.name] = currentMaterial;
            currentMaterial.n = 0;

            char name[256];
            sscanf(line, "%*s%s", name);
            currentMaterial.name = string(name);
        default:
            return;
    }
    currentMaterial.commands[currentMaterial.n] = c;
    int i = currentMaterial.n * 4;
    memcpy(currentMaterial.arguments + i, args, sizeof (args));
}

char* findSlash(char* line, int& n, char** indices) {
    while (*line) {
        if (*line == '/') {
            // remove the slash and put a '\0'. That creates a substring.
            *line = '\0';
            indices[n++] = line + 1;
            break;
        }
        line++;
    }
    return line;
}

void WFObjectLoader::loadOBJ(char* line) {
    char name[20];
    if (sscanf(line,  "%s", name) < 1) {
        return;
    }
    // printf("name \"%s\"\n", name);
    if (strcmp(name, "v") == 0) {
        float x, y, z, w = 1;
        // v <x> <y> <z> [w]
        sscanf(line, "%*s%f%f%f%f", &x, &y, &z, &w);
        arguments.push_back(x);
        arguments.push_back(y);
        arguments.push_back(z);
        arguments.push_back(w);
        commands.push_back(glVertex4fv);
    } else if (strcmp(name, "vt") == 0) {
        float u, v = 0, w = 0;
        // v <x> <y> <z> [w]
        sscanf(line, "%*s%f%f%f", &u, &v, &w);
        arguments.push_back(u);
        arguments.push_back(v);
        arguments.push_back(w);
        arguments.push_back(0);
        commands.push_back(glTexCoord3fv);
    } else if (strcmp(name, "vn") == 0) {
        float x, y, z;
        // v <x> <y> <z> [w]
        sscanf(line, "%*s%f%f%f", &x, &y, &z);
        arguments.push_back(x);
        arguments.push_back(y);
        arguments.push_back(z);
        arguments.push_back(0);
        commands.push_back(glNormal3fv);
    } else if (strcmp(name, "f") == 0) {
        line++;
        put(WFObject::begin);
        while (*line != '\0' && *line != '\n') {
            // f <v>/[vt]/[vn]
            int v, vt, vn;

            // split by '/'
            char* indices[3];

            int n = 0;

            indices[n++] = line;

            line = findSlash(line, n, indices);
            line = findSlash(line, n, indices);

            sscanf(indices[0], "%d", &v);

            if (sscanf(indices[1], "%d", &vt)) {
                put(glTexCoord3fv, textures, vt);
            }
            if (sscanf(indices[2], "%d", &vn)) {
                put(glNormal3fv, normals, vn);
            }

            put(glVertex4fv, vertices, v);
        }
        put(WFObject::end);
    } else if (strcmp(name, "newmtl") == 0) {
        char name[256];
        sscanf(line, "%*s%s", name);
        loadMaterial(name);
    } else if (strcmp(name, "usemtl") == 0) {
        char name[256];
        sscanf(line, "%*s%s", name);
        useMaterial(name);
    }
}

void WFObjectLoader::put(WFCommand c, vector<float>& v, int index) {
    if (index < 0) {
        index += v.size();
    }
    index *= 4;
    commands.push_back(c);
    for (int i = 0; i < 4; i++) {
        arguments.push_back(v[index++]);
    }
}

void WFObjectLoader::put(WFCommand c) {
    commands.push_back(c);
    for (int i = 0; i < 4; i++) {
        arguments.push_back(0);
    }
}

void WFObjectLoader::useMaterial(const char* name) {
    string s(name);

    WFMaterial& m = materials[s];

    for (int i = 0, k = 0; i < m.n; i++) {
        commands.push_back(m.commands[i]);
        arguments.push_back(m.arguments[k++]);
        arguments.push_back(m.arguments[k++]);
        arguments.push_back(m.arguments[k++]);
        arguments.push_back(m.arguments[k++]);
    }
}

void WFObjectLoader::forEachLine(const char* filename, void (*c)(char*, WFObjectLoader&)) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        string msg = "Could not open: ";
        msg += filename;
        throw IOException(msg);
    }
    char line[512];
    while (fgets(line, sizeof (line), file) != NULL) {
        c(line, *this);
    }
    fclose(file);
}
