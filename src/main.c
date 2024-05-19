#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "vector.h"
#include "display.h"

#define N_POINTS 9 * 9 * 9

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Declare an array of vectors/points
/////////////////////////////////////////////////////////////////////////////////////////////////////
float fov_factor = 128;
vec3_t cube_points[N_POINTS]; // 9*9*9 cube (An array of structs)
vec2_t projected_points[N_POINTS];

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

//////////////////////////////////////////////////////////////////////
// Function that recives a 3D vector and returns a projected 2D point
//////////////////////////////////////////////////////////////////////
vec2_t project(vec3_t point)
{
	vec2_t projected_point = {
		.x = (fov_factor * point.x),  // ranging from -128 to 128
		.y = (fov_factor * point.y)}; // ranging from -128 to 128

	return projected_point;
}

void create2dPoints(void)
{
	for (int i = 0; i < N_POINTS; i++)
	{
		vec3_t point = cube_points[i];

		// Project the current point (read them one by one)
		vec2_t projected_point = project(point);

		// Save the projectd 2D vector in the array of projected points
		projected_points[i] = projected_point;
	}
}

void update(void)
{
}

void render(void)
{
	// draw_grid();

	// Loop all projectd points and render then
	printf("width: %d\n", window_width);
	printf("height: %d\n", window_height);
	for (int i = 0; i < N_POINTS; i++)
	{
		vec2_t projected_point = projected_points[i];
		draw_rect(
			projected_point.x + (window_width / 2),
			projected_point.y + (window_height / 2),
			4,
			4,
			0xFFFF0000);
	}

	render_color_buffer();
	clear_color_buffer(0xFFFFFF00);

	// draw_pixel(10, 10, 0xFFFF0000);
	// draw_rect(200, 100, 300, 300, 0xFFF0F0F0);

	SDL_RenderPresent(renderer);
}

int main(void)
{
	printf("size: %ld\n", sizeof(cube_points));
	/* Crate a SDL window */
	is_running = initilize_window();

	setup();
	create2dPoints();

	while (is_running)
	{
		process_input();
		update();
		render();
	}

	destroy_window();
	return 0;
}
