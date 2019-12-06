/* 
 * File:   cube.h
 * Author: 2019202344
 *
 * Created on 6 de Dezembro de 2019, 18:01
 */

#ifndef CUBE_H
#define CUBE_H

class cube_t {
private:
    static GLfloat normals[6][3];
    static GLint faces[6][4];
    static GLfloat vertices[8][3]; /* Will be filled in with X,Y,Z vertexes. */
    static GLfloat color[6][3];

    static bool sStart;
    
    static bool init();
    
public:
    static void drawBox(void);

};

#endif /* CUBE_H */

