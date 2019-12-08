/* 
 * File:   textrendering.h
 * Author: 2019202344
 *
 * Created on 8 de Novembro de 2019, 16:46
 */

#ifndef TEXTRENDERING_H
#define TEXTRENDERING_H

#include <GL/gl.h>

void drawText(const char* text);

void measureText(const char* text, int* width, int* height);

#endif /* TEXTRENDERING_H */

