#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "array.h"
#include "sorting.h"
#include "vector.h"
#include "matrix.h"
#include "display.h"
#include "triangle.h"
#include "mesh.h"
#include "light.h"

/////////////////////////////////////////////////////////////////////////////////////////////
// Array of triangles that should be rendered frame by frame
/////////////////////////////////////////////////////////////////////////////////////////////
triangle_t *triangles_to_render = NULL;

vec3_t camera_position = {0, 0, 0};

mat4_t projection_matrix;

bool is_running = false;
int previous_frame_time = 0;

enum render_methods render_method;
enum cull_methods cull_method;

void setup(void)
{
	render_method = RENDER_FILL_TRIANGLE;
	cull_method = CULL_BACKFACE;

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

	// Initialize the perspective projection matrix
	projection_matrix = mat4_make_perspective(PI / 3.0f, (float)window_height / (float)window_width, 0.1, 100.0);

	// Loads the cube values in the mesh data structure
	// load_cube_mesh_data();
	// load_obj_file_data("./assets/f-22.obj");
	load_obj_file_data("./assets/fighter-jet.obj");
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
			is_running = false;
		if (event.key.keysym.sym == SDLK_1)
			render_method = RENDER_WIRE_VERTEX;
		if (event.key.keysym.sym == SDLK_2)
			render_method = RENDER_WIRE;
		if (event.key.keysym.sym == SDLK_3)
			render_method = RENDER_FILL_TRIANGLE;
		if (event.key.keysym.sym == SDLK_4)
			render_method = RENDER_FILL_TRIANGLE_WIRE;
		if (event.key.keysym.sym == SDLK_5)
			render_method = RENDER_FILL_TRIANGLE_WIRE_VERTEX;
		if (event.key.keysym.sym == SDLK_c)
			cull_method = CULL_BACKFACE;
		if (event.key.keysym.sym == SDLK_d)
			cull_method = CULL_NONE;
		break;
	}
}

void draw_mesh(void)
{
	// Initialize the array of triangles to render
	triangles_to_render = NULL;

	mesh.rotation.x += 0.01;
	// mesh.rotation.y += 0.01;
	// mesh.rotation.z += 0.01;

	mesh.translation.z = 5;
	//  mesh.translation.x += 0.1f;
	//   mesh.translation.x += 0.01;
	//     mesh.scale.x += 0.002;
	//     mesh.scale.y += 0.002;

	// Create a scale matrix that will be used to multiply the mesh vertices
	mat4_t scale_matrix = mat4_make_scale(mesh.scale.x, mesh.scale.y, mesh.scale.z);
	mat4_t rotation_matrix_x = mat4_make_rotation(mesh.rotation.x, AXIS_X);
	mat4_t rotation_matrix_y = mat4_make_rotation(mesh.rotation.y, AXIS_Y);
	mat4_t rotation_matrix_z = mat4_make_rotation(mesh.rotation.z, AXIS_Z);
	mat4_t translation_matrix = mat4_make_translation(mesh.translation.x, mesh.translation.y, mesh.translation.z);

	// Loop all triangle faces of our mesh
	for (int i = 0; i < array_length(mesh.faces); i++)
	{
		face_t mesh_face = mesh.faces[i];

		vec3_t face_vertices[3];
		face_vertices[0] = mesh.vertices[mesh_face.a - 1];
		face_vertices[1] = mesh.vertices[mesh_face.b - 1];
		face_vertices[2] = mesh.vertices[mesh_face.c - 1];

		// Loop all three vertices of this current face and apply transformation
		vec4_t transformed_vertices[3];
		for (int j = 0; j < 3; j++)
		{
			vec4_t transformed_vertex = vec4_from_vec3(face_vertices[j], 1);

			// Create a World matrix combining scale, rotation, adn translation matrices
			mat4_t world_matrix = mat4_identity();
			world_matrix = mat4_mul_mat4(scale_matrix, world_matrix);
			world_matrix = mat4_mul_mat4(rotation_matrix_x, world_matrix);
			world_matrix = mat4_mul_mat4(rotation_matrix_y, world_matrix);
			world_matrix = mat4_mul_mat4(rotation_matrix_z, world_matrix);
			world_matrix = mat4_mul_mat4(translation_matrix, world_matrix);

			// Multiply all matrices and load world matrix
			transformed_vertex = mat4_mul_vec4(transformed_vertex, world_matrix);

			transformed_vertices[j] = transformed_vertex;
		}

		// Calculating normal vector
		vec3_t vertex_A = vec3_from_vec4(transformed_vertices[0]);
		vec3_t vertex_B = vec3_from_vec4(transformed_vertices[1]);
		vec3_t vertex_C = vec3_from_vec4(transformed_vertices[2]);

		vec3_t vector_AB = vec3_sub(vertex_A, vertex_B);
		vec3_normalize(&vector_AB);
		vec3_t vector_AC = vec3_sub(vertex_A, vertex_C);
		vec3_normalize(&vector_AC);

		vec3_t normal_vector = vec3_cross(vector_AC, vector_AB);
		vec3_normalize(&normal_vector);

		if (cull_method == CULL_BACKFACE)
		{
			vec3_t vector_OB = vec3_sub(camera_position, vertex_B);
			vec3_normalize(&vector_OB);
			float camera_normal_dot = vec3_dot(vector_OB, normal_vector);

			if (camera_normal_dot > 0)
			{
				continue; // Do not do the rest in the for loop
			}
		}

		vec4_t projected_point[3];
		for (int j = 0; j < 3; j++)
		{
			// Project the current vertex
			// projected_point[j] = project(vec3_from_vec4(transformed_vertices[j]));
			projected_point[j] = mat4_mul_vec4_project(projection_matrix, transformed_vertices[j]);

			// Scale to the view
			projected_point[j].x *= (window_width / 2.0f);
			projected_point[j].y *= (window_height / 2.0f);
			// projected_point[j].y += 500;

			// Invert the y values to account for flipped screen y coordinate
			projected_point[j].y *= -1;

			// Scale and translate the projected points to the middle of the screen
			projected_point[j].x += (window_width / 2.0f);
			projected_point[j].y += (window_height / 2.0f);
		};
		float avg_depth = (transformed_vertices[0].z + transformed_vertices[1].z + transformed_vertices[2].z) / 3;
		triangle_t projected_triangle = {
			.points = {
				{.x = projected_point[0].x, .y = projected_point[0].y},
				{.x = projected_point[1].x, .y = projected_point[1].y},
				{.x = projected_point[2].x, .y = projected_point[2].y}},
			//.color = mesh_face.color,
			.color = light_apply_intensity(mesh_face.color, SDL_clamp(vec3_dot(normal_vector, global_light.direction), 0.0, 1.0)),
			.avg_depth = avg_depth};

		// if (i == 0)
		// {
		// 	printf("%f: \n", vec3_dot(normal_vector, global_light.direction));
		// }

		// save the projeted triangle in the array of triangles to render
		// triangles_to_render[i] = projected_triangle;

		array_push(triangles_to_render, projected_triangle);
	}
	// Sort triangles based on their z-value
	if (array_length(triangles_to_render) > 0)
	{
		bubblesort(triangles_to_render, array_length(triangles_to_render));
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

	draw_mesh();

	draw_grid();
}

void render(void)
{
	// printf("Triangles to render count: %d\n", array_length(triangles_to_render));
	//  For loop drawing triangles' points, lines and render them
	for (int i = 0; i < array_length(triangles_to_render); i++)
	{
		triangle_t triangle = triangles_to_render[i];
		// Drawing Filled Triangles
		if (render_method == RENDER_FILL_TRIANGLE || render_method == RENDER_FILL_TRIANGLE_WIRE || render_method == RENDER_FILL_TRIANGLE_WIRE_VERTEX)
		{
			// Draw filled triangles
			draw_filled_triangle(
				triangle.points[0].x, triangle.points[0].y,
				triangle.points[1].x, triangle.points[1].y,
				triangle.points[2].x, triangle.points[2].y,
				triangle.color);
		}
		// Drawing Wires
		if (render_method == RENDER_WIRE || render_method == RENDER_WIRE_VERTEX || render_method == RENDER_FILL_TRIANGLE_WIRE || render_method == RENDER_FILL_TRIANGLE_WIRE_VERTEX)
		{
			// Draw unfield triangle
			draw_triangle(
				triangle.points[0].x, triangle.points[0].y,
				triangle.points[1].x, triangle.points[1].y,
				triangle.points[2].x, triangle.points[2].y,
				0xFF000000);
		}
		// Drawing Vertices
		if (render_method == RENDER_FILL_TRIANGLE_WIRE_VERTEX || render_method == RENDER_WIRE_VERTEX)
		{
			draw_rect(triangle.points[0].x, triangle.points[0].y, 5, 5, 0xFF0000FF);
			draw_rect(triangle.points[1].x, triangle.points[1].y, 5, 5, 0xFF0000FF);
			draw_rect(triangle.points[2].x, triangle.points[2].y, 5, 5, 0xFF0000FF);
		}
	}

	// Clear the array of triangles to render every frame loop
	array_free(triangles_to_render);

	render_color_buffer();
	clear_color_buffer(0xFF399145);

	SDL_RenderPresent(renderer);
}

// Free the memory that was dynamically allocated by the program
void free_resources(void)
{
	free(color_buffer);
	array_free(mesh.faces);
	array_free(mesh.vertices);
}

int main(void)
{
	/* Crate an SDL window */
	is_running = initilize_window();

	setup();

	while (is_running)
	{
		process_input();
		update();
		render();
	}

	destroy_window();
	free_resources();
	return 0;
}
