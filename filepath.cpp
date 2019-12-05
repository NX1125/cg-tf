
#include <ostream>
#include <iostream>

#include "filepath.h"

#define OS_SEPARATOR '/'

using namespace std;

const char* getHome() {
    const char* home = getenv("HOME");
    if (home == nullptr) {
        throw "Could not find \"home\" with getenv";
    }
    return home;
}

file_path::file_path(const char* path) {
    if (path[0] == '~') {
        // it is at home.
        add(getHome());
        if (path[1] != '\0') {
            // TODO: Check if path[1] is a path separator indeed
            // there is more to the path afterwards.
            add(&path[2]);
        }
    } else {
        add(path);
    }
}

file_path::~file_path() {
}

void file_path::append(const char* component) {
    add(component);

    valid = false;
}

void file_path::add(const char* path) {
    // find all OS_SEPARATOR from the path.

    const char* offset = path;
    size_t length = 0;

    while (offset[length] != '\0') {
        switch (offset[length]) {
            case '\\':
            case '/':
                // create a string from the start of the char*
                components.push_back(string(offset, length));
                // reset the current length
                offset = &offset[length + 1];
                length = 0;
                break;
            default:
                length++;
                break;
        }
    }

    if (length > 0) {
        components.push_back(string(offset, length));
    }
}

const char* file_path::toString() {
    if (!valid) {
        valid = true;
        s.clear();

        for (string& c : components) {
            s += c;
            s += OS_SEPARATOR;
        }
        // remove the last slash
        s.resize(s.length() - 1);
    }
    
    return s.c_str();
}

void file_path::debug() {
    for (string& s : components) {
        cout << "component: " << s << endl;
    }
}

void file_path::removeLast() {
    components.pop_back();
    valid = false;
}
