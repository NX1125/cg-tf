/* 
 * File:   filepath.h
 * Author: guilh
 *
 * Created on 2 de outubro de 2019, 23:56
 */

#ifndef FILEPATH_H
#define FILEPATH_H

#include <vector>
#include <string>

class file_path {
private:
    std::vector<std::string> components;

    bool valid;

    std::string s;
public:
    file_path(const char* path);

    void debug();

    void append(const char* component);
    
    void append(const std::string& component) {
        append(component.c_str());
    }

    const char* toString();

private:
    void add(const char* path);
};

#endif /* FILEPATH_H */

