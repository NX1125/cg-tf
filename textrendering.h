/* 
 * File:   textrendering.h
 * Author: 2019202344
 *
 * Created on 8 de Novembro de 2019, 16:46
 */

#ifndef TEXTRENDERING_H
#define TEXTRENDERING_H

#include <GL/gl.h>

void rasterChars(GLfloat x, GLfloat y, GLfloat z, const char* text, double r, double g, double b);

void drawText(GLfloat x, GLfloat y, const char* text, double r, double g, double b);

void measureText(const unsigned char* text, int* width, int* height);

#endif /* TEXTRENDERING_H */

