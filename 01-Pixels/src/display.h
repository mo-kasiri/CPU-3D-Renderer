#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

extern SDL_Window *window;
extern SDL_Renderer *renderer;

extern SDL_Texture *color_buffer_texture;
extern uint32_t *color_buffer;

extern unsigned int window_width;
extern unsigned int window_height;

bool initialize_window(void);
void draw_grid(const unsigned int size);
void draw_rectangle(const int x, const int y, const unsigned int width, const unsigned int height, const u_int32_t color);

void render_color_buffer(void);
void clear_color_buffer(uint32_t color);
void destroy_window(void);

#endif