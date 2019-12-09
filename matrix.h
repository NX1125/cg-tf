/* 
 * File:   matrix.h
 * Author: gg
 *
 * Created on 8 de dezembro de 2019, 20:43
 */

#ifndef MATRIX_H
#define MATRIX_H

#include <GL/gl.h>

namespace mtx {

    /**
     * A 4x4 matrix.
     */
    struct matrix4x4_t {
        /**
         * A column-major matrix.
         */
        GLfloat matrix[16] = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1,
        };
    };
}

#endif /* MATRIX_H */

