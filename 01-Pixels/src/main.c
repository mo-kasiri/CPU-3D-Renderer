#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "display.h"
#include "vector.h"

#define N_POINTS 9 * 9 * 9
vec3_t cube_points[N_POINTS];
vec2_t projected_points[N_POINTS];

vec3_t camera_position = {.x = 0, .y = 0, .z = -5};

const float fov_scale = 640.f;

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
        SDL_GetError();
    }

    // Start loading my array of vectors
    // From -1 to 1 (in this 9*9*9 cube)
    int point_count = 0;
    for (float x = -1; x <= 1; x += 0.25)
    {
        for (float y = -1; y <= 1; y += 0.25)
        {
            for (float z = -1; z <= 1; z += 0.25)
            {
                vec3_t new_point = {x, y, z};
                cube_points[point_count++] = new_point;
            }
        }
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
/////////////////////////////////////////////////////////////
/// Function that receives 3D points and returns 2D points
/////////////////////////////////////////////////////////////
vec2_t project(vec3_t point)
{
    vec2_t projected_point = {
        .x = (fov_scale * point.x) / point.z,  // / point.z,
        .y = (fov_scale * point.y) / point.z}; // / point.z};
    return projected_point;
}

void update(void)
{
    for (int i = 0; i < N_POINTS; i++)
    {
        vec3_t point = cube_points[i];

        point.z -= camera_position.z;

        projected_points[i] = project(point);
    }
}

void render(void)
{
    // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    // SDL_RenderClear(renderer);

    clear_color_buffer(0xFF00d4ff);
    draw_grid(10);

    for (int i = 0; i < N_POINTS; i++)
    {
        vec2_t projected_point = projected_points[i];
        draw_rectangle(projected_point.x + window_width / 2, projected_point.y + window_height / 2, 4, 4, 0xFFFFFF00);
    }

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
