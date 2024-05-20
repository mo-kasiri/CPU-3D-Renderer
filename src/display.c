#include "display.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
uint32_t *color_buffer = NULL;
SDL_Texture *color_buffer_texture = NULL;
int window_width = 1920;
int window_height = 1080;

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

	if (!display_mode.w && !display_mode.h)
	{
		window_width = display_mode.w;
		window_height = display_mode.h;
		// ratio = window_width / window_height;
	}

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
		fprintf(stderr, "Error creating SDL renderer.\n");
		return false;
	}

	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	return true;
}

void draw_pixel(const int x, const int y, uint32_t color)
{
	if (x >= 0 && x < window_width && y >= 0 && y < window_height)
		color_buffer[(window_width * y) + x] = color;
}

void draw_rect(const unsigned int posx, const unsigned int posy, const unsigned int width, const unsigned int height, const uint32_t color)
{

	for (unsigned int i = 0; i < width; i++)
	{
		for (unsigned int j = 0; j < height; j++)
		{
			int current_x = posx + i;
			int current_y = posy + j;
			draw_pixel(current_x, current_y, color);
			// if (x >= posx && x <= posx + width && y >= posy && y <= posy + height)
			// color_buffer[(window_width * y) + x] = color;
		}
	}
}

void draw_grid(void)
{
	for (unsigned int y = 0; y < window_height; y++)
	{
		for (unsigned int x = 0; x < window_width; x++)
		{
			if (y % 20 == 0 && x % 20 == 0)
			{
				color_buffer[(window_width * y) + x] = 0xFF333333;
			}
		}
	}
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
void clear_color_buffer(uint32_t color)
{
	for (unsigned int y = 0; y < window_height; y++)
	{
		for (unsigned int x = 0; x < window_width; x++)
		{
			color_buffer[(window_width * y) + x] = color;
		}
	}
}

void destroy_window(void)
{
	free(color_buffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
