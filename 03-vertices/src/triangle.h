#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "vector.h"

typedef struct
{
    int a; // first index of vertex of the face
    int b; // second index of vertex of the face
    int c; // third index of vertex of the face
    //int indices[3];
} face_t;  // (triangle faces) Stores vertex index

typedef struct
{
    vec2_t points[3];
} triangle_t; // Stores the actual vec2 points of the triangle in the screen [{x,y}, {x,y}, {x,y}]
#endif