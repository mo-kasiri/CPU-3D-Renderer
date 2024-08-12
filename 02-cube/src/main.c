#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "vector.h"
#include "display.h"

////////////////////////////////////////////////////////
// Writing some text on the screen
///////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Declare an array of vectors/points
/////////////////////////////////////////////////////////////////////////////////////////////////////
#define N_POINTS 9 * 9 * 9
vec3_t camera_position = {.x = 0, .y = 0, .z = -3};
vec3_t cube_rotation = {.x = 0, .y = 0, .z = 0};
float fov_factor = 640;
vec3_t cube_points[N_POINTS]; // 9*9*9 cube (An array of structs)
vec2_t projected_points[N_POINTS];

bool is_running = false;
int previous_frame_time = 0;

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
		.x = fov_factor * point.x / point.z,  // ranging from -128 to 128
		.y = fov_factor * point.y / point.z}; // ranging from -128 to 128

	return projected_point;
}

void drawCube(void)
{
	cube_rotation.x += 0.02;
	cube_rotation.y += 0.02;
	cube_rotation.z += 0.02;

	for (int i = 0; i < N_POINTS; i++)
	{
		vec3_t transformed_point = vec3_rotate_x(cube_points[i], cube_rotation.x);
		transformed_point = vec3_rotate_y(transformed_point, cube_rotation.y);
		transformed_point = vec3_rotate_z(transformed_point, cube_rotation.z);

		// Translate the points away from the camera
		vec3_t newTransformedPoint = transformed_point;
		transformed_point.z -= camera_position.z;

		// Project the current point (read them one by one)
		vec2_t projected_point = project(transformed_point);

		// Save the projectd 2D vector in the array of projected points
		projected_points[i] = projected_point;

		draw_rect(
			projected_point.x + (window_width / 2),
			projected_point.y + (window_height / 2),
			-4 * (newTransformedPoint.z - 2),
			-4 * (newTransformedPoint.z - 2),
			// 4,
			// 4,
			0xFFFFFF00);
	}
}

void update(void)
{
	// Wait some time untill reach the target frame time in milliseconds
	int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);

	// Only delay execuation if we are running too fast
	if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
	{
		SDL_Delay(time_to_wait);
	}
	// How many milli seconds has passed since the last frame
	previous_frame_time = SDL_GetTicks(); // Started since SDL_Init

	drawCube();
	// text();
}

void render(void)
{
	draw_grid();

	// Loop all projectd points and render then
	// printf("width: %d\n", window_width);
	// printf("height: %d\n", window_height);

	// For loop for drawing rectangles
	// for (int i = 0; i < N_POINTS; i++)
	// {
	// 	vec2_t projected_point = projected_points[i];
	// 	draw_rect(
	// 		projected_point.x + (window_width / 2),
	// 		projected_point.y + (window_height / 2),
	// 		-4 * (cube_points[i].z - 2),
	// 		-4 * (cube_points[i].z - 2),
	// 		// 4,
	// 		// 4,
	// 		0xFFFFFF00);
	// }

	render_color_buffer();
	clear_color_buffer(0xFF399145);

	// draw_pixel(10, 10, 0xFFFF0000);
	// draw_rect(200, 100, 300, 300, 0xFFF0F0F0);

	SDL_RenderPresent(renderer);
}

int main(void)
{
	// printf("size: %ld\n", sizeof(cube_points));
	/* Crate a SDL window */
	is_running = initilize_window();

	setup();
	// create2dPoints();

	while (is_running)
	{
		process_input();
		update();
		render();
	}

	destroy_window();
	return 0;
}
