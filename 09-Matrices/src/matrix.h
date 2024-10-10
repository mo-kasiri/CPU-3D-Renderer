#ifndef MATRIX_H
#define MATRIX_H
#include "vector.h"

typedef struct
{
    float m[4][4];
} mat4_t;

enum axis
{
    AXIS_X,
    AXIS_Y,
    AXIS_Z,
};

mat4_t mat4_identity(void);
mat4_t mat4_make_scale(float sx, float sy, float sz);
mat4_t mat4_make_translation(float tx, float ty, float tz);
mat4_t mat4_make_rotation(float r, enum axis a);

vec4_t mat4_mul_vec4(vec4_t v, mat4_t m);

#endif