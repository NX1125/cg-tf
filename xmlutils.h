/* 
 * File:   xmlutils.h
 * Author: guilh
 *
 * Created on 3 de outubro de 2019, 10:33
 */

#ifndef XMLUTILS_H
#define XMLUTILS_H

#include <exception>
#include <string>

#include "tinyxml/tinyxml.h"

using namespace std;

class MissingSVGShapeException : public std::exception {
private:
    std::string name;
    std::string msg;

public:

    MissingSVGShapeException(const char* name) :
    exception(), name(name) {
        msg += "Missing <";
        msg += name;
        msg += ">";
    }

    virtual ~MissingSVGShapeException() noexcept {
    }

    const char* what() const noexcept {
        return msg.c_str();
    }
};

class IOException : public exception {
private:
    string msg;

public:

    IOException(const string& s) :
    exception(), msg(s) {
    }

    IOException(const char* msg) :
    exception(), msg(msg) {
    }

    virtual ~IOException() noexcept {
    }

    const char* what() const noexcept {
        return msg.c_str();
    }
};

class AttributeException : public exception {
private:
    int type;

public:

    AttributeException(int type) :
    exception(), type(type) {
    }

    virtual ~AttributeException() noexcept {
    }

    const char* what() const noexcept {
        switch (type) {
            case TIXML_NO_ATTRIBUTE:
                return "missing attribute";
            case TIXML_WRONG_TYPE:
                return "wrong type";
            default:
                return "unknown";
        }
    }
};

class MissingElementException : public exception {
private:
    string tag;
    string msg;

public:

    MissingElementException(const string& s) :
    exception(), tag(s) {
        msg = "Missing tag <" + s + ">";
    }

    MissingElementException(const char* tag) :
    exception(), tag(tag) {
        msg = string("Missing tag <") + tag + ">";
    }

    virtual ~MissingElementException() noexcept {
    }

    const char* what() const noexcept {
        return msg.c_str();
    }
};

double readDoubleAttribute(const TiXmlElement* e, const char* name);

const char* readAttribute(const TiXmlElement* e, const char* name);

const char* getTextOrThrow(const TiXmlElement* e);

#endif /* XMLUTILS_H */

