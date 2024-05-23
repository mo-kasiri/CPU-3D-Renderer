#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vector.h"

typedef struct
{
    int a;
    int b;
    int c;
} face_t; // Stores vertex index

typedef struct
{
    vec2_t points[3];
} triangle_t; // Stores the actual vec2 points of the triangle in the screen {{x,y}, {x,y}, {x,y}}
#endif