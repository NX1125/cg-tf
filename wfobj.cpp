
#include "wfobj.h"

void wf_object_t::draw() {
    float* k = arguments;
    for (int i = 0; i < n; i++, k += 4) {
        // printf("c(%f, %f, %f, %f)\n", k[0], k[1], k[2], k[3]);
        commands[i].apply(k);
    }
}

wf_object_t* wf_object_loader_t::load(const char* filename) {
    file_path fp(filename);
    filepath = &fp;
    forEachLine(filename, loadOBJ);
    filepath = NULL;

    return build();
}

wf_object_t* wf_object_loader_t::build() {
    WFCommand* cs = (WFCommand*) malloc(commands.size() * sizeof (WFCommand));
    float* args = (float*) malloc(arguments.size() * sizeof (float));

    for (int i = 0; i < commands.size(); i++) {
        cs[i] = commands[i];
    }
    for (int i = 0; i < arguments.size(); i++) {
        args[i] = arguments[i];
    }

    printf("%ld commands loaded\n", commands.size());

    return new wf_object_t(cs, args, commands.size());
}

void wf_object_loader_t::loadMaterial(const char* filename) {
    printf("Opening %s material file\n", filename);
    filepath->removeLast();
    filepath->append(filename);
    forEachLine(filepath->toString(), loadMTL);
    materials[currentMaterial.name] = currentMaterial;

    printf("There are %ld materials in %s\n", materials.size(), filename);
}

void wf_object_loader_t::loadMTL(char* line) {
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
            sscanf(line, "%*s%f%f%f", args, args + 1, args + 2);
            args[3] = 1.0f;
            switch (name[1]) {
                case 'a':
                    c.apply = wf_object_t::ambient;
                    c.name = "ambient";
                    break;
                case 'd':
                    c.apply = wf_object_t::diffuse;
                    c.name = "diffuse";
                    break;
                case 's':
                    c.apply = wf_object_t::specular;
                    c.name = "specular";
                    break;
            }
            break;
        case 'N':
            sscanf(line, "Ns %f", args);
            args[1] = 0;
            args[2] = 0;
            args[3] = 0;
            c.apply = wf_object_t::shininess;
            c.name = "shininess";
            break;
        case 'n':
            if (!currentMaterial.name.empty()) {
                materials[currentMaterial.name] = currentMaterial;
            }
            currentMaterial.n = 0;

            char name[256];
            sscanf(line, "%*s%s", name);
            currentMaterial.name = string(name);

            printf("Loaded material %s\n", name);
        default:
            return;
    }
    currentMaterial.commands[currentMaterial.n++] = c;
    int i = currentMaterial.n * 4;
    memcpy(currentMaterial.arguments + i, args, sizeof (args));
}

void wf_object_loader_t::loadOBJ(char* line) {
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
    } else if (strcmp(name, "mtllib") == 0) {
        char name[256];
        sscanf(line, "%*s%s", name);
        loadMaterial(name);
    } else if (strcmp(name, "usemtl") == 0) {
        char name[256];
        sscanf(line, "%*s%s", name);
        useMaterial(name);
    }
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

void wf_object_loader_t::parseFace(char* line) {
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

    put(wf_object_t::begin, "begin");

    for (int i = 0; i < faceVertices.size(); i++) {
        wf_face_vertex_t vertex = faceVertices[i];

        if (vertex.texture != 0) {
            put(glTexCoord3fv, "texture", textures, vertex.texture);
        }
        if (vertex.normal != 0) {
            put(glNormal3fv, "normal", normals, vertex.normal);
        }
        put(glVertex4fv, "vertex", vertices, vertex.vertex);
    }

    put(wf_object_t::end, "end");
}

void wf_object_loader_t::put(WFFunction c, const char* name,
        vector<float>& v, int index) {
    WFCommand f = {c, name};
    if (index < 0) {
        index += v.size();
    } else {
        index--;
    }
    index *= 4;
    commands.push_back(f);
    // printf("%f %f %f %f\n", v[0], v[1], v[2], v[3]);
    for (int i = 0; i < 4; i++) {
        arguments.push_back(v[index++]);
    }
}

void wf_object_loader_t::put(WFFunction c, const char* name) {
    WFCommand f = {c, name};
    commands.push_back(f);
    for (int i = 0; i < 4; i++) {
        arguments.push_back(0);
    }
}

void wf_object_loader_t::useMaterial(const char* name) {
    string s(name);

    wf_material_t& m = materials[s];

    for (int i = 0, k = 0; i < m.n; i++) {
        commands.push_back(m.commands[i]);
        arguments.push_back(m.arguments[k++]);
        arguments.push_back(m.arguments[k++]);
        arguments.push_back(m.arguments[k++]);
        arguments.push_back(m.arguments[k++]);
    }
}

void wf_object_loader_t::forEachLine(const char* filename, void (*c)(char*, wf_object_loader_t&)) {
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

void wf_object_t::ambient(const GLfloat* coords) {
    glMaterialfv(GL_FRONT, GL_AMBIENT, coords);
}

void wf_object_t::diffuse(const GLfloat* coords) {
    glMaterialfv(GL_FRONT, GL_DIFFUSE, coords);
}

void wf_object_t::specular(const GLfloat* coords) {
    glMaterialfv(GL_FRONT, GL_SPECULAR, coords);
}

void wf_object_t::shininess(const GLfloat* coords) {
    glMaterialfv(GL_FRONT, GL_SHININESS, coords);
}

void wf_object_t::begin(const GLfloat* ignore) {
    glBegin(GL_POLYGON);
}

void wf_object_t::end(const GLfloat* ignore) {
    glEnd();
}