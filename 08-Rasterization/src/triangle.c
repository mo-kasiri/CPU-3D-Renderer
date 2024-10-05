#include "triangle.h"

// TODO: Craete implementation for triangle.h functions
void swap(int *a, int*b){
    int tmp = *a;
    *b = *a;
    *a = tmp;
}

// Fill flat-bottom triangle
void fill_flatBottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color){

}

// Fill flat-top triangle
void fill_flatTop_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color){

}

/*/////////////////////////////////////////////////////////////////
// Deviding any rectangle into two flat-bottom and flat-top trangle

                     (x1,y1)
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
                                 (x3,y3)

///////////////////////////////////////////////////////////////////*/

void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2,int y2, uint32_t color ){
    // We need to sort vertices by y-coordinates (y0 < y1 < y2)
    if(y0 > y1){
        swap(&y0,&y1);
        swap(&x0,&x1);
    }
    if(y1>y2){
        swap(&y1,&y2);
        swap(&x1,&x2);
    }
    if(y0 > y1){
        swap(&y0,&y1);
        swap(&x0,&x1);
    }
    
    int My = y1;
    int Mx = ((float)(x2-x1) + (y1-y0) - (float)(y2 - y0)) + x0;

    // Fill flat-bottom triangle
    fill_flatBottom_triangle();

    // Fill flat-top triangle
    fill_flatTop_triangle();


}