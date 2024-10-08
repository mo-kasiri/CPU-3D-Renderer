#include "triangle.h"
#include "display.h"

// TODO: Craete implementation for triangle.h functions
void swap(int *a, int *b)
{
  int tmp = *a;
  *a = *b;
  *b = tmp;
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
    swap(&y0, &y1);
    swap(&x0, &x1);
  }
  if (y1 > y2)
  {
    swap(&y1, &y2);
    swap(&x1, &x2);
  }
  if (y0 > y1)
  {
    swap(&y0, &y1);
    swap(&x0, &x1);
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