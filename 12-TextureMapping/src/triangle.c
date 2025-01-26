#include "triangle.h"
#include "display.h"
#include "sorting.h"

/*/////////////////////////////////////////////////////////////////
// Deviding any rectangle into two flat-bottom and flat-top trangle

                     (x0,y0)
                       / \
                      /   \
                     /     \
                    /       \
                (x1,y1)----(Xm, Ym)
                      \_      \
                        \_     \
                          \_    \
                            \_   \
                              \_  \
                                \_ \
                                  \_\
                                 (x2,y2)

///////////////////////////////////////////////////////////////////*/

void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
  // We need to sort vertices by y-coordinates (y0 < y1 < y2)
  if (y0 > y1)
  {
    swap_vertices(&y0, &y1);
    swap_vertices(&x0, &x1);
  }
  if (y1 > y2)
  {
    swap_vertices(&y1, &y2);
    swap_vertices(&x1, &x2);
  }
  if (y0 > y1)
  {
    swap_vertices(&y0, &y1);
    swap_vertices(&x0, &x1);
  }

  if (y1 == y2)
  {
    fill_flatBottom_triangle(x0, y0, x1, y1, x2, y2, color);
  }
  else if (y1 == y0)
  {
    // Fill flat-bottom triangle
    fill_flatTop_triangle(x0, y0, x1, y1, x2, y2, color);
  }
  else
  {
    int My = y1;
    int Mx = ((float)((x2 - x0) * (y1 - y0)) / (float)(y2 - y0)) + x0;

    // Fill flat-top triangle
    fill_flatTop_triangle(x1, y1, Mx, My, x2, y2, color);

    // Fill flat-bottom triangle
    fill_flatBottom_triangle(x0, y0, x1, y1, Mx, My, color);
  }
}

// Fill flat-top triangle

/*         (x0,y0)------(x1,y1)
               \          /
                \        /
                 \      /
                  \    /
                   \  /
                  (x2,y2)

*/
void fill_flatTop_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
  float inv_slop1 = (float)(x2 - x0) / (y2 - y0);
  float inv_slop2 = (float)(x2 - x1) / (y2 - y1);

  float startingPointX = x2;
  float endingPointX = x2;

  for (int y = y2; y >= y0; y--)
  {
    draw_line(startingPointX, y, endingPointX, y, color);
    startingPointX -= inv_slop2;
    endingPointX -= inv_slop1;
  }
}

// Fill flat-bottom triangle

/*                (x0, y0)
                    /\
                   /  \
                  /    \
                 /      \
                /        \
            (x1,y1)----(x2,y2)
*/
void fill_flatBottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{

  float inv_slop1 = (float)(x1 - x0) / (y1 - y0);
  float inv_slop2 = (float)(x2 - x0) / (y2 - y0);

  float starting_pointX = x0;
  float ending_pointX = x0;

  for (int y = y0; y <= y2; y++)
  {
    draw_line(starting_pointX, y, ending_pointX, y, color);
    starting_pointX += inv_slop1;
    ending_pointX += inv_slop2;
  };
}

////////////////////////////////////////////////////////////////////////////
// Draw a textured triangle with the flat-top and flat-bottom triangle
// We split the original triangle in two, half flat-top and half flat-bottom
///////////////////////////////////////////////////////////////////////////
/*

                     v0,uv0
                       #
                      ###
                     #####
            v1,uv1 ####### vm,uvm
                      #####
                        ####
                          ###
                            ##
                              #
                            v2,uv2

*/

void draw_texture_triangle(int x0, int y0, int x1, int y1, int x2, int y2, tex2_t uv0, tex2_t uv1, tex2_t uv3, uint32_t *texture)
{
  // We need to sort vertices by y-coordinates (y0 < y1 < y2) => y increases from top to bottom
  if (y0 > y1)
  {
    swap_vertices(&y0, &y1);
    swap_vertices(&x0, &x1);
    swap_tex2_t(&uv0, &uv1);
  }
  if (y1 > y2)
  {
    swap_vertices(&y1, &y2);
    swap_vertices(&x1, &x2);
    swap_tex2_t(&uv1, &uv3);
  }
  if (y0 > y1)
  {
    swap_vertices(&y0, &y1);
    swap_vertices(&x0, &x1);
    swap_tex2_t(&uv0, &uv1);
  }

  if ((y1 != y0) | (y1 != y2) | (y2 != y1))
  {
    if (y1 == y2)
    {
      draw_flatBottom_textured_triangle(x0, y0, x1, y1, x2, y2, uv0, uv1, uv3, texture);
    }
    else if (y1 == y0)
    {
      draw_flatTop_textured_triangle(x0, y0, x1, y1, x2, y2, uv0, uv1, uv3, texture);
    }
    // else
    // {
    //   // int My = y1;
    //   // int Mx = ((float)((x2 - x0) * (y1 - y0)) / (float)(y2 - y0)) + x0;

    //   // // Fill flat-top triangle
    //   // draw_flatTop_textured_triangle(x0, y0, Mx, My, x2, y2, uv0, uv1, uv3, texture);

    //   // // Fill flat-bottom triangle
    //   // draw_flatBottom_textured_triangle(x0, y0, x1, y1, Mx, My, uv0, uv1, uv3, texture);
    // }
  }
}

/*         v0,uv0        v1,uv1
               ##########
                ########
                 ######
                  ####
                   ##
                 v2,uv2

*/

void draw_flatTop_textured_triangle(int x0, int y0, int x1, int y1, int x2, int y2, tex2_t uv0, tex2_t uv1, tex2_t uv3, uint32_t *texture)
{

  float inv_slop01 = (float)(x2 - x0) / (y2 - y0);
  float inv_slop12 = (float)(x2 - x1) / (y2 - y1);
  // printf("slopes: %f %f\n", inv_slop01, inv_slop12);

  int x_start, x_end = 0;

  for (int y = y0; y <= y2; y++)
  {
    x_start = x0 + (y - y0) * inv_slop01;
    x_end = x1 + (y - y1) * inv_slop12;

    // printf("start: %d, end: %d \n", x_start, x_end);

    if (x_start > x_end)
      swap_int(&x_start, &x_end);

    for (int x = x_start; x < x_end; x++)
    {
      draw_pixel(x, y, 0xFFFF00FF);
    }
  }
}

// Fill flat-bottom triangle

/*               v0,uv0
                   ##
                  ####
                 ######
                ########
               ##########
            v1,uv1     v2,uv2
*/
void draw_flatBottom_textured_triangle(int x0, int y0, int x1, int y1, int x2, int y2, tex2_t uv0, tex2_t uv1, tex2_t uv3, uint32_t *texture)
{

  float inv_slop10 = (float)(x0 - x1) / (y0 - y1);
  float inv_slop02 = (float)(x0 - x2) / (y0 - y2);
  // printf("slopes: %f %f\n", inv_slop10, inv_slop02);

  int x_start, x_end = 0;

  for (int y = y2; y >= y0; y--)
  {
    x_start = x1 + (y - y1) * inv_slop10;
    x_end = x2 + (y - y2) * inv_slop02;

    // printf("start: %d, end: %d \n", x_start, x_end);

    if (x_start > x_end)
      swap_int(&x_start, &x_end);

    for (int x = x_start; x < x_end; x++)
    {
      draw_pixel(x, y, 0xFF0000FF);
    }
  }
}
