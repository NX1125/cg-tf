/* 
 * File:   settings.h
 * Author: guilh
 *
 * Created on 3 de outubro de 2019, 10:00
 */

#ifndef SETTINGS_H
#define SETTINGS_H

#include <fstream>
#include <string>

#include "tinyxml/tinyxml.h"
#include "simplesvg.h"

/**
 * The settings for the game application. This settings consists of a path
 * for the SVG and a velocity factor.
 */
struct app_settings {
    /**
     * The path of the SVG file, already concatenated.
     */
    std::string filenameSVG;

    /**
     * The velocity factor. This is used to multiply the velocity at the end
     * of the airstrip.
     */
    double vel;
    
    double bulletVel;
    
    double eBulletVel;
    double eBulletF;
    double eVel;
    
    simple_svg* svg;

    app_settings(const char* filename);

    app_settings() {
    }
    
    ~app_settings();

private:
    void read(std::istream& file, const char* filename);

    /**
     * This is used to find the {@code <jogador>} and
     * {@code <arquivoDaArena>} tags within the root.
     * 
     * @param root The {@code <aplicacao>} element.
     */
    void read(const TiXmlElement* root);

    /**
     * Reads the velocity factor from the element.
     * 
     * The element should be {@code <jogador>}.
     * 
     * @param root The {@code <jogador>} element
     */
    void readPlayer(const TiXmlElement* root);

    /**
     * Reads the {@code <arquivoDaArena>}. This reads the SVG path elements
     * and concatenate them.
     * 
     * @param root The {@code <arquivoDaArena>} element.
     */
    void readSVG(const TiXmlElement* root);
    
    void readEnemy(const TiXmlElement* root);
    
    void debugFields();
    
    void parseSVG();
};

#endif /* SETTINGS_H */

