#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <stdint.h>
#include <stdbool.h>
#include "vector.h"
#include "texture.h"

typedef struct // Stores vertex index {a,b,c} & color of the face & uv coordinates
{
    int a;
    int b;
    int c;
    tex2_t a_uv;
    tex2_t b_uv;
    tex2_t c_uv;
    uint32_t color;
} face_t;

typedef struct // Stores the actual vec2 points of the triangle in the screen {{x,y}, {x,y}, {x,y}}
{
    vec2_t points[3];
    tex2_t tex_coords[3];
    uint32_t color;
    float avg_depth;
} triangle_t;
#endif

void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
void fill_flatBottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
void fill_flatTop_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);

void draw_texel(int x, int y, uint32_t *texture, vec2_t point_a, vec2_t point_b, vec2_t point_c, tex2_t uv0, tex2_t uv1, tex2_t uv2);
void draw_texture_triangle(int x0, int y0, int x1, int y1, int x2, int y2, tex2_t uv0, tex2_t uv1, tex2_t uv3, uint32_t *texture);
void draw_flatTop_textured_triangle(int x0, int y0, int x1, int y1, int x2, int y2, tex2_t uv0, tex2_t uv1, tex2_t uv2, uint32_t *texture);
void draw_flatBottom_textured_triangle(int x0, int y0, int x1, int y1, int x2, int y2, tex2_t uv0, tex2_t uv1, tex2_t uv2, uint32_t *texture);

vec3_t barycentric_weights(vec2_t a, vec2_t b, vec2_t c, vec2_t p);

void swap_vertices(int *a, int *b);