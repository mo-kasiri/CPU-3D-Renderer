#include "sorting.h"
#include "stdio.h"

void swap_int(int *a, int *b)
{
    int temp = *b;
    *b = *a;
    *a = temp;
}

// TODO: Craete implementation for triangle.h functions
void swap_vertices(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void swap_tex2_t(tex2_t *a, tex2_t *b)
{
    tex2_t tmp = *a;
    *a = *b;
    *b = tmp;
}

void swap_triangle(triangle_t *xp, triangle_t *yp)
{
    triangle_t tmp = *xp;
    *xp = *yp;
    *yp = tmp;
}

void bubblesort(triangle_t arr[], int n)
{
    int i, j;
    bool swapped;
    for (i = 0; i < n - 1; i++)
    {
        swapped = false;
        for (j = 0; j < n - i - 1; j++)
        {
            if (arr[j].avg_depth < arr[j + 1].avg_depth)
            {
                swap_triangle(&arr[j], &arr[j + 1]);
                swapped = true;
            }
        }

        // If no two elements were swapped by inner loop,
        // then break
        if (swapped == false)
            break;
    }
}