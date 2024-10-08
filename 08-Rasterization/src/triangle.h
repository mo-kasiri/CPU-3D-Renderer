#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <stdint.h>
#include <stdbool.h>
#include "vector.h"

typedef struct // Stores vertex index {a,b,c}
{
    int a;
    int b;
    int c;
    uint32_t color;
} face_t;

typedef struct // Stores the actual vec2 points of the triangle in the screen {{x,y}, {x,y}, {x,y}}
{
    vec2_t points[3];
    uint32_t color;
    float avg_depth;
} triangle_t;
#endif

void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
void fill_flatBottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
void fill_flatTop_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);

void swap_vertices(int *a, int *b);