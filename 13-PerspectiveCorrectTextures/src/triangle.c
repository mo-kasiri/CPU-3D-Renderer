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

///////////////////////////////////////////////////////////////////////////////
// Function to draw the textured pixel at position x and y using interpolation
//////////////////////////////////////////////////////////////////////////////
int var = 0;
void draw_texel(int x, int y, uint32_t *texture, vec2_t point_a, vec2_t point_b, vec2_t point_c, tex2_t uv0, tex2_t uv1, tex2_t uv2)
{
  vec2_t point_p = {x, y};
  vec3_t weights = barycentric_weights(point_a, point_b, point_c, point_p);

  float alpha = weights.x;
  float beta = weights.y;
  float gamma = weights.z;

  // Perform the interpolation of all U and V values using barycentric weights
  float interpolated_u = uv0.u * alpha + uv1.u * beta + uv2.u * gamma; // between 0 , 1
  float interpolated_v = uv0.v * alpha + uv1.v * beta + uv2.v * gamma; // between 0, 1

  // Map the UV coordinates to the full texture width and height
  int texture_x = abs((int)(interpolated_u * texture_width));
  int texture_y = abs((int)(interpolated_v * texture_height));

  if ((texture_width * texture_y) + texture_x < 4096)
    draw_pixel(x, y, rgba_to_argb(texture[(texture_width * texture_y) + texture_x]));
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

void draw_texture_triangle(int x0, int y0, int x1, int y1, int x2, int y2, tex2_t uv0, tex2_t uv1, tex2_t uv2, uint32_t *texture)
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
    swap_tex2_t(&uv1, &uv2);
  }
  if (y0 > y1)
  {
    swap_vertices(&y0, &y1);
    swap_vertices(&x0, &x1);
    swap_tex2_t(&uv0, &uv1);
  }

  vec2_t point_a = {x0, y0};
  vec2_t point_b = {x1, y1};
  vec2_t point_c = {x2, y2};

  // Top Part of the triangle

  /*         v0,uv0        v1,uv1
               ##########
                ########
                 ######
                  ####
                   ##
                 v2,uv2

*/
  float inv_slop_1 = 0;
  float inv_slop_2 = 0;

  if (y1 - y0 != 0)
    inv_slop_1 = (float)(x1 - x0) / abs(y1 - y0);
  if (y2 - y0 != 0)
    inv_slop_2 = (float)(x2 - x0) / abs(y2 - y0);

  if (y1 - y0 != 0)
  {
    for (int y = y0; y <= y1; y++)
    {
      int x_start = x1 + (y - y1) * inv_slop_1;
      int x_end = x0 + (y - y0) * inv_slop_2;

      if (x_start > x_end)
        swap_int(&x_start, &x_end);

      for (int x = x_start; x < x_end; x++)
      {

        draw_texel(x, y, texture, point_a, point_b, point_c, uv0, uv1, uv2);
      }
    }
  }

  // Bottom Part of the triangle

  // Fill flat-bottom triangle

  /*               v0,uv0
                     ##
                    ####
                   ######
                  ########
                 ##########
              v1,uv1     v2,uv2
  */
  inv_slop_1 = 0;
  inv_slop_2 = 0;

  if (y2 - y1 != 0)
    inv_slop_1 = (float)(x2 - x1) / abs(y2 - y1);
  if (y2 - y0 != 0)
    inv_slop_2 = (float)(x2 - x0) / abs(y2 - y0);

  if (y2 - y1 != 0)
  {
    for (int y = y1; y <= y2; y++)
    {
      int x_start = x1 + (y - y1) * inv_slop_1;
      int x_end = x0 + (y - y0) * inv_slop_2;

      if (x_start > x_end)
        swap_int(&x_start, &x_end);

      for (int x = x_start; x < x_end; x++)
      {
        draw_texel(x, y, texture, point_a, point_b, point_c, uv0, uv1, uv2);
      }
    }
  }
}

// Per pixel calculation
/*
           B
          /|\
         / b \
        /  |  \
       /  (p)  \
      / a/ | \g \
     / /   |   \ \
    A-------------C

*/
vec3_t barycentric_weights(vec2_t a, vec2_t b, vec2_t c, vec2_t p)
{
  vec2_t ac = vec2_sub(c, a);
  vec2_t ab = vec2_sub(b, a);
  vec2_t pc = vec2_sub(c, p);
  vec2_t pb = vec2_sub(b, p);
  vec2_t ap = vec2_sub(p, a);

  // Area of the full parallelogram (triangle ABC) using cross product
  float area_parallelogram_abc = (ac.x * ab.y - ac.y * ab.x); // || AC x AB || => 2D determinant

  // Alpha = area of parallelogram-PBC over the area of the full parallelogram-ABC
  float alpha = (pc.x * pb.y - pc.y * pb.x) / area_parallelogram_abc;

  // Beta = area of parallelogram-APC over the area of the full parallelogram-ABC
  float beta = (ac.x * ap.y - ac.y * ap.x) / area_parallelogram_abc;

  // Gamma
  float gama = 1.0 - alpha - beta;

  vec3_t weights = {alpha, beta, gama};
  return weights;
}
