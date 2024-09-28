#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vector.h"

typedef struct // Stores vertex index {a,b,c}
{
    int a;
    int b;
    int c;
} face_t;

typedef struct // Stores the actual vec2 points of the triangle in the screen {{x,y}, {x,y}, {x,y}}
{
    vec2_t points[3];
} triangle_t;
#endif