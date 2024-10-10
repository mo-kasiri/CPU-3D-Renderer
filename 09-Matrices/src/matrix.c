#include "matrix.h"
#include <math.h>
#include <stdio.h>

mat4_t mat4_identity(void)
{
    // | 1 0 0 0 |
    // | 0 1 0 0 |
    // | 0 0 1 0 |
    // | 0 0 0 1 |
    mat4_t m = {{{1, 0, 0, 0},
                 {0, 1, 0, 0},
                 {0, 0, 1, 0},
                 {0, 0, 0, 1}}};
    return m;
}

mat4_t mat4_make_scale(float sx, float sy, float sz)
{
    // | sx, 0, 0, 0 |
    // | 0, sy, 0, 0 |
    // | 0, 0, sz, 0 |
    // | 0, 0,  0, 1 |
    mat4_t m = {{{sx, 0, 0, 0},
                 {0, sy, 0, 0},
                 {0, 0, sz, 0},
                 {0, 0, 0, 1}}};
    return m;
}

mat4_t mat4_make_translation(float tx, float ty, float tz)
{
    // mat4_t result = mat4_identity();
    // result.m[0][3] = tx;
    // result.m[1][3] = ty;
    // result.m[2][3] = tz;
    mat4_t result = {{{1, 0, 0, tx},
                      {0, 1, 0, ty},
                      {0, 0, 1, tz},
                      {0, 0, 0, 1}}};
    return result;
}

vec4_t mat4_mul_vec4(vec4_t v, mat4_t m)
{
    vec4_t result;
    result.x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3] * v.w;
    result.y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3] * v.w;
    result.z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3] * v.w;
    result.w = m.m[3][0] * v.x + m.m[3][1] * v.y + m.m[3][2] * v.z + m.m[3][3] * v.w;
    return result;
}

mat4_t mat4_make_rotation(float r, enum axis a)
{
    float c = cos(r);
    float s = sin(r);
    if (a == AXIS_X)
    {
        mat4_t result = {{{1, 0, 0, 0},
                          {0, c, -s, 0},
                          {0, s, c, 0},
                          {0, 0, 0, 1}}};
        return result;
    }
    else if (a == AXIS_Y)
    {
        mat4_t result = {{{c, 0, s, 0},
                          {0, 1, 0, 0},
                          {-s, 0, c, 0},
                          {0, 0, 0, 1}}};
        return result;
    }
    else
    {
        mat4_t result = {{{c, -s, 0, 0},
                          {s, c, 0, 0},
                          {0, 0, 1, 0},
                          {0, 0, 0, 1}}};
        return result;
    };
};
