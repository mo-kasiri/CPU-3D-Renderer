#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "vector.h"
#include "display.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Declare an array of vectors/points
/////////////////////////////////////////////////////////////////////////////////////////////////////

const int N_POINTS = 9 * 9 * 9;
vec3_t cube_points[N_POINTS]; // 9*9*9 cube

bool is_running = false;

void setup(void)
{
	// Allocate the required memory in bytes to bold the color buffer
	color_buffer = (uint32_t *)malloc(sizeof(uint32_t) * window_width * window_height);
	if (color_buffer == NULL)
		fprintf(stderr, "Error creating color buffer.\n");

	// Creating a SDL texture that is used to display the color buffer
	color_buffer_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		0,
		window_width,
		window_height);

	// Start loading my array of vectors
	// From -1 to 1 (in this 9*9*9 cube)

	int point_count = 0;

	for (float x = -1.0; x <= 1.0; x += 0.25)
	{
		for (float y = -1.0; y <= 1.0; y += 0.25)
		{
			for (float z = -1.0; z <= 1.0; z += 0.25)
			{
				vec3_t new_point = {.x = x, .y = y, .z = z};
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

void update(void)
{
}

void render(void)
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderClear(renderer);

	render_color_buffer();

	clear_color_buffer(0xFFFFFF00);

	draw_grid();
	draw_pixel(10, 10, 0xFFFF0000);
	draw_rect(200, 100, 300, 300, 0xFFF0F0F0);

	SDL_RenderPresent(renderer);
}

int main(void)
{
	/* Crate a SDL window */
	is_running = initilize_window();

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
