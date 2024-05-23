#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "vector.h"
#include "display.h"
#include "mesh.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Declare an array of vectors/points
/////////////////////////////////////////////////////////////////////////////////////////////////////

triangle_t triangles_to_render[N_MESH_FACES];

vec3_t camera_position = {.x = 0, .y = 0, .z = -5};
vec3_t cube_rotation = {.x = 0, .y = 0, .z = 0};

float fov_factor = 640;

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

	// Loop all triangle faces of our mesh
	for (int i = 0; i < N_MESH_FACES; i++)
	{
		face_t mesh_face = mesh_faces[i];

		vec3_t face_vertices[3];
		face_vertices[0] = mesh_vertices[mesh_face.a - 1];
		face_vertices[1] = mesh_vertices[mesh_face.b - 1];
		face_vertices[2] = mesh_vertices[mesh_face.c - 1];

		triangle_t projected_triangle;

		// Loop all three vertices of this current face and apply transformation
		for (int j = 0; j < 3; j++)
		{
			vec3_t transformed_vertex = face_vertices[j];

			transformed_vertex = vec3_rotate_x(transformed_vertex, cube_rotation.x);
			transformed_vertex = vec3_rotate_y(transformed_vertex, cube_rotation.y);
			transformed_vertex = vec3_rotate_z(transformed_vertex, cube_rotation.z);

			// Translate the vertex away from the camera
			transformed_vertex.z -= camera_position.z;

			// Project the current vertex
			vec2_t projected_point = project(transformed_vertex);

			// Scale and translate the projected points to the middel of the screen
			projected_point.x += (window_width / 2);
			projected_point.y += (window_height / 2);

			projected_triangle.points[j] = projected_point;
		}
		// save the projeted triangle in the array of triangles to render
		triangles_to_render[i] = projected_triangle;
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
}

void render(void)
{
	draw_grid();

	// Loop all projectd points and render then
	// printf("width: %d\n", window_width);
	// printf("height: %d\n", window_height);

	// For loop for drawing rectangles and render them
	for (int i = 0; i < N_MESH_FACES; i++)
	{
		triangle_t triangle = triangles_to_render[i];
		draw_rect(triangle.points[0].x, triangle.points[0].y, 3, 3, 0xFFFFFF00);
		draw_rect(triangle.points[1].x, triangle.points[1].y, 3, 3, 0xFFFFFF00);
		draw_rect(triangle.points[2].x, triangle.points[2].y, 3, 3, 0xFFFFFF00);
	}

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
