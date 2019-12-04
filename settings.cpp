
#include "settings.h"

#include "xmlutils.h"
#include "filepath.h"

const TiXmlElement* wrapMissingElementException(const TiXmlNode* root, const char* name) {
    const TiXmlElement* elem = root->FirstChildElement(name);
    if (elem == NULL) {
        throw MissingElementException(name);
    }
    return elem;
}

app_settings::~app_settings() {
    if (svg != NULL) {
        delete svg;
    }
}

app_settings::app_settings(const char* filename) {
    // TODO: What is RAII again?

    FILE* f = fopen(filename, "r");

    if (f == NULL) {
        throw IOException("Could not open file");
    }
    try {
        TiXmlDocument doc;

        if (doc.LoadFile(f)) {
            read(wrapMissingElementException(&doc, "aplicacao"));

            debugFields();

            parseSVG();
        } else {
            throw IOException(doc.ErrorDesc());
        }
        fclose(f);
    } catch (...) {
        if (f != NULL) {
            // printf("Closing file\n");
            fclose(f);
        }

        throw;
    }
}

void app_settings::debugFields() {
    printf(" *** Loaded config file\n");

    printf("svg file to load: %s\n", filenameSVG.c_str());

    // player
    printf("Player\n");
    printf("velocity factor        = %f\n", vel);
    printf("bullet velocity factor = %f\n", bulletVel);

    // enemy
    printf("Enemy\n");
    printf("enemy velocity factor        = %f\n", eVel);
    printf("enemy bullet velocity factor = %f\n", eBulletVel);
    printf("bullet frequency (bullets/s) = %f\n", eBulletF);

    printf(" ***\n");
}

void app_settings::parseSVG() {
    svg = new simple_svg(filenameSVG.c_str());

    printf("There are %u lines\n", (unsigned int) svg->lines.size());
    printf("There are %u circles\n", (unsigned int) svg->circles.size());

    svg->lines[0].debug();
}

void app_settings::read(const TiXmlElement* root) {
    // query the element for the player
    readPlayer(wrapMissingElementException(root, "jogador"));
    readEnemy(wrapMissingElementException(root, "inimigo"));
    // query for the location element
    readSVG(wrapMissingElementException(root, "arquivoDaArena"));
}

void app_settings::readPlayer(const TiXmlElement* root) {
    // tries to read the vel attribute
    vel = readDoubleAttribute(root, "vel");
    bulletVel = readDoubleAttribute(root, "velTiro");
}

void app_settings::readEnemy(const TiXmlElement* root) {
    // tries to read the vel attribute
    eVel = readDoubleAttribute(root, "vel");
    eBulletVel = readDoubleAttribute(root, "velTiro");
    eBulletF = readDoubleAttribute(root, "freqTiro");
}

string findTextElement(const TiXmlElement* parent, const char* name) {
    return getTextOrThrow(wrapMissingElementException(parent, name));
}

void app_settings::readSVG(const TiXmlElement* root) {
    string name = findTextElement(root, "nome");
    string extension = findTextElement(root, "tipo");
    string folder = findTextElement(root, "caminho");

    file_path path(folder.c_str());

    path.append(name + "." + extension);

    filenameSVG = string(path.toString());
}
