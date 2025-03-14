#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define FPS 30
#define FRAME_TARGET_TIME (1000 / FPS) // The Constant Delta

typedef enum
{
    CULL_NONE,
    CULL_BACKFACE
} cull_methods;

typedef enum
{
    RENDER_WIRE,
    RENDER_WIRE_VERTEX,
    RENDER_FILL_TRIANGLE,
    RENDER_FILL_TRIANGLE_WIRE,
    RENDER_FILL_TRIANGLE_WIRE_VERTEX,
    RENDER_TEXTURE,
    RENDER_TEXTURE_WIRE,
    RENDER_TEXTURE_WIRE_VERTEX
} render_methods;

// extern enum render_methods render_method;
// extern enum cull_methods cull_method;

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern uint32_t *color_buffer;
extern SDL_Texture *color_buffer_texture;
extern int window_width;
extern int window_height;
extern float ratio;

bool initialize_window(void);
void draw_grid(void);
void draw_pixel(const int x, const int y, uint32_t color);
void draw_rect(const unsigned int posx,
               const unsigned int posy,
               const unsigned int width,
               const unsigned int height,
               const uint32_t color);

void draw_line(const int x0, const int y0, const int x1, const int y1, const uint32_t color);
void draw_triangle(const int x0, const int y0, const int x1, const int y1, const int x2, const int y2, const uint32_t color);

void render_color_buffer(void);
void clear_color_buffer(uint32_t color);
void destroy_window(void);

#endif