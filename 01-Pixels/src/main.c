#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "display.h"

bool is_running = false;

void setup(void)
{
    color_buffer = (u_int32_t *)malloc(sizeof(uint32_t) * window_width * window_height);

    /// Creating a SDL texture that is use to display the color buffer
    color_buffer_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        window_width,
        window_height);

    if (color_buffer_texture == NULL)
    {
        fprintf(stderr, "Error creating color buffer texture.\n");
    }
}

void process_input(void)
{
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type)
    {
    case SDL_QUIT:
        is_running = false;
        break;

    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE)
        {
            is_running = false;
            break;
        }
    }
}

// The main goal is to copy data from color buffer (that we created and populated with data) to color texture of SDL library
void render_color_buffer(void)
{
    SDL_UpdateTexture(
        color_buffer_texture,
        NULL,
        color_buffer,
        (int)window_width * sizeof(uint32_t));

    SDL_RenderCopy(
        renderer,
        color_buffer_texture,
        NULL,
        NULL);
}

void update(void)
{
}

void render(void)
{
    // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderClear(renderer);

    clear_color_buffer(0xFFFFFF00);
    draw_pixel(10, 10, 0xFFFFFFFF);
    draw_grid(10);
    draw_rectangle(100, 200, 100, 200, 0xFFFF0000);

    render_color_buffer();
    SDL_RenderPresent(renderer);
}

int main()
{

    is_running = initialize_window();

    setup();

    while (is_running)
    {
        process_input();
        update();
        render();
    }

    destroy_window();

    return 0;
}
