
#include "xmlutils.h"

double readDoubleAttribute(const TiXmlElement* e, const char* name) {
    double v;
    int result = e->QueryDoubleAttribute(name, &v);
    if (result != TIXML_SUCCESS) {
        throw AttributeException(result);
    }
    return v;
}

const char* readAttribute(const TiXmlElement* e, const char* name) {
    const char* value = e->Attribute(name);
    if (value == NULL) {
        throw AttributeException(TIXML_NO_ATTRIBUTE);
    }
    return value;
}

const char* getTextOrThrow(const TiXmlElement* e) {
    const char* text = e->GetText();
    if (text == NULL) {
        throw IOException(e->Value());
    }
    return text;
}
