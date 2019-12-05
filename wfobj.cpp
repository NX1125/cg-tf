
#include "wfobj.h"

void WFObject::draw() {
    float* k = arguments;
    for (int i = 0; i < n; i++, k += 4) {
        // printf("c(%f, %f, %f, %f)\n", k[0], k[1], k[2], k[3]);
        commands[i](k);
    }
}

WFObject* WFObjectLoader::build() {
    WFCommand* cs = (WFCommand*) malloc(commands.size() * sizeof (WFCommand));
    float* args = (float*) malloc(arguments.size() * sizeof (float));

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

char* findSlash(char* line, int n, char** indices) {
    while (*line) {
        if (*line == '/') {
            indices[n] = line + 1;
            return line + 1;
        }
        line++;
    }
    return line;
}

void WFObjectLoader::loadOBJ(char* line) {
    char name[20];
    if (sscanf(line, "%s", name) < 1) {
        return;
    }
    // printf("line: %s", line);
    // printf("name \"%s\"\n", name);
    if (strcmp(name, "v") == 0) {
        float x, y, z, w = 1;
        // v <x> <y> <z> [w]
        sscanf(line, "%*s%f%f%f%f", &x, &y, &z, &w);
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(z);
        vertices.push_back(w);
    } else if (strcmp(name, "vt") == 0) {
        float u, v = 0, w = 0;
        // v <x> <y> <z> [w]
        sscanf(line, "%*s%f%f%f", &u, &v, &w);
        textures.push_back(u);
        textures.push_back(v);
        textures.push_back(w);
        textures.push_back(0);
    } else if (strcmp(name, "vn") == 0) {
        float x, y, z;
        // v <x> <y> <z> [w]
        sscanf(line, "%*s%f%f%f", &x, &y, &z);
        normals.push_back(x);
        normals.push_back(y);
        normals.push_back(z);
        normals.push_back(0);
    } else if (strcmp(name, "f") == 0) {
        parseFace(line);
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

void WFObjectLoader::parseFace(char* line) {
    char* source = line;

    enum wf_token_type_t {
        INDEX, SLASH
    };

    struct wf_token_t {
        wf_token_type_t type;
        int index;
    };

    line++; // skip 'f'

    // token analysis
    vector<wf_token_t> tokens;

    while (*line) {
        char c = *line;
        // check if it is a digit or the sign of an index
        if (c == '-' || '0' <= c && c <= '9') {
            wf_token_t token;
            token.type = wf_token_type_t::INDEX;
            sscanf(line, "%d", &token.index);
            // line++; // consume char
            do {
                line++;
                // continue to consume the char while it is a digit
            } while ('0' <= *line && *line <= '9');
            tokens.push_back(token);
            continue;
        } else if (c == '/') {
            // SLASH!
            wf_token_t token;
            token.type = wf_token_type_t::SLASH;
            tokens.push_back(token);
        } else if (c == '\n') {
            break;
        }
        line++;
    }

    //printf("begin\n");
    //for (int i = 0; i < tokens.size(); i++) {
    //    switch (tokens[i].type) {
    //        case wf_token_type_t::INDEX:
    //            printf("index = %d\n", tokens[i].index);
    //            break;
    //        case wf_token_type_t::SLASH:
    //            printf("slash\n");
    //            break;
    //    }
    //}

    // syntax analysis

    // This class represents one of the v/vt/vn sequence.

    struct wf_face_vertex_t {
        int vertex;
        int texture;
        int normal;
    };

    vector<wf_face_vertex_t> faceVertices;

    for (int i = 0; i < tokens.size(); i++) {
        if (tokens[i].type == wf_token_type_t::INDEX) {
            struct wf_face_vertex_t vertex = {tokens[i].index, 0, 0};
            if (i + 1 < tokens.size() &&
                    tokens[i + 1].type == wf_token_type_t::SLASH) {
                i++;
                if (tokens[i + 1].type == wf_token_type_t::INDEX) {
                    i++;
                    vertex.texture = tokens[i].index;
                }
            }
            if (i + 1 < tokens.size() &&
                    tokens[i + 1].type == wf_token_type_t::SLASH) {
                i++;
                if (tokens[i + 1].type == wf_token_type_t::INDEX) {
                    i++;
                    vertex.normal = tokens[i].index;
                }
            }

            faceVertices.push_back(vertex);
        }
    }

    // If we parsed correctly, we are capable of generating a formatted version
    // of the source. Considering that the source is also formatted, the code
    // below should be equal to the source.
    string build = "f";
    for (int i = 0; i < faceVertices.size(); i++) {
        wf_face_vertex_t v = faceVertices[i];

        build += " ";
        build += to_string(v.vertex);
        if (v.texture != 0) {
            build += "/";
            build += to_string(v.texture);
        }
        if (v.normal != 0) {
            if (v.texture == 0) {
                build += "/";
            }
            build += "/";
            build += to_string(v.normal);
        }
    }
    build += "\n";

    if (strcmp(build.c_str(), source) != 0) {
        printf("DIFFERENT: \"%s\" != \"%s\"", build.c_str(), source);
    }

    // semantic analysis

    put(WFObject::begin);

    for (int i = 0; i < faceVertices.size(); i++) {
        wf_face_vertex_t vertex = faceVertices[i];

        if (vertex.texture != 0) {
            put(glTexCoord3fv, textures, vertex.texture);
        }
        if (vertex.normal != 0) {
            put(glNormal3fv, normals, vertex.normal);
        }
        put(glVertex4fv, vertices, vertex.vertex);
    }

    put(WFObject::end);
}

void WFObjectLoader::put(WFCommand c, vector<float>& v, int index) {
    if (index < 0) {
        index += v.size();
    }
    index *= 4;
    commands.push_back(c);
    // printf("%f %f %f %f\n", v[0], v[1], v[2], v[3]);
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
