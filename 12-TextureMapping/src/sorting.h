#ifndef SORTING_H
#define SORTING_H
#include "triangle.h"

// TODO: Craete implementation for triangle.h functions
void swap_int(int *a, int *b);
void swap_vertices(int *a, int *b);
void swap_tex2_t(tex2_t *a, tex2_t *b);
void swap_triangle(triangle_t *xp, triangle_t *yp);
void bubblesort(triangle_t arr[], int n);

#endif