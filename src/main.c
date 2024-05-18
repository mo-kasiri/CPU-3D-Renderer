#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

bool is_running;
SDL_Window *window;
SDL_Renderer *renderer;

uint32_t *color_buffer = NULL;
SDL_Texture *color_buffer_texture = NULL;

int window_width = 800;
int window_height = 600;

bool initilize_window(void)
{

	if (SDL_Init(SDL_INIT_EVERYTHING == -1))
	{
		fprintf(stderr, "Error initilazing SDL.\n");
		return false;
	}

	// Use SDL to query what is the fullscreen max. width and height
	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(0, &display_mode);

	// if (!display_mode.w && !display_mode.h)
	// {
	// 	window_width = display_mode.w;
	// 	window_height = display_mode.h;
	// }

	printf("width: %d\n", window_width);
	printf("height: %d\n", window_height);

	// Create a SDL window
	window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		window_width,
		window_height,
		SDL_WINDOW_BORDERLESS); // SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE

	if (!window)
	{
		fprintf(stderr, "Error creating SDL window.\n");
		return false;
	}

	//  Create a SDL renderer
	renderer = SDL_CreateRenderer(window, -1, 0);

	if (!renderer)
	{
		fprintf(stderr, "Error creating SDL renderer. ", SDL_GetError(), " .\n");
		return false;
	}

	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	return true;
}

void drawRect(const unsigned int posx, const unsigned int posy, const unsigned int width, const unsigned int height, const uint32_t color)
{

	for (unsigned int y = 0; y < window_height; y++)
	{
		for (unsigned int x = 0; x < window_width; x++)
		{
			if (x >= posx && x <= posx + width && y >= posy && y <= posy + height)
				color_buffer[(window_width * y) + x] = color;
		}
	}
}

void drawGrid(void)
{
	for (unsigned int y = 0; y < window_height; y++)
	{
		for (unsigned int x = 0; x < window_width; x++)
		{
			if (y % 20 == 0 || x % 20 == 0)
			{
				color_buffer[(window_width * y) + x] = 0xFF000000;
			}
		}
	}
}

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

void update(void)
{
}

void render_color_buffer(void)
{
	// Copies data from color buffer to texture buffer
	SDL_UpdateTexture(
		color_buffer_texture,
		NULL,
		color_buffer,
		(int)(window_width * sizeof(uint32_t)));

	// Copy a portion of the texture to the current rendering target.
	SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

// Populating color buffer with a desired value
void clear_color_buffer(uint32_t *color)
{
	for (unsigned int y = 0; y < window_height; y++)
	{
		for (unsigned int x = 0; x < window_width; x++)
		{
			color_buffer[(window_width * y) + x] = *color;
		}
	}
}

void render(void)
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderClear(renderer);

	render_color_buffer();

	uint32_t color = 0xFFFFFF00;
	clear_color_buffer(&color);

	drawGrid();
	drawRect(200, 100, 300, 300, 0xFFF0F0F0);

	SDL_RenderPresent(renderer);
}

void destroy_window(void)
{
	free(color_buffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int main(void)
{
	/* Crate a SDL window */
	is_running = initilize_window();

	setup();

	while (is_running)
	{
		process_input();
		// update();
		render();
	}

	destroy_window();
	return 0;
}
