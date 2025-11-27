#include "display.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
uint32_t *color_buffer = NULL;
SDL_Texture *color_buffer_texture = NULL;
int window_width = 1920;
int window_height = 1080;

bool initialize_window(void)
{
	SDL_Init(SDL_INIT_VIDEO);
	if (SDL_Init(SDL_INIT_EVERYTHING == -1))
	{
		fprintf(stderr, "Error initilazing SDL.\n");
		return false;
	}

	// Use SDL to query what is the fullscreen max. width and height
	if (SDL_GetNumVideoDisplays() > 0)
	{
		static unsigned int display_in_use = 0; /* Only using first display */

		int i, display_mode_count;
		SDL_DisplayMode mode;
		SDL_DisplayMode *modes = (SDL_DisplayMode *)malloc(sizeof(SDL_DisplayMode) * SDL_GetNumDisplayModes(display_in_use));
		// Uint32 f;

		// SDL_Log("SDL_GetNumVideoDisplays(): %i", SDL_GetNumVideoDisplays());

		display_mode_count = SDL_GetNumDisplayModes(display_in_use);

		// SDL_Log("SDL_GetNumDisplayModes: %i", display_mode_count);

		for (i = 0; i < display_mode_count; ++i)
		{
			if (SDL_GetDisplayMode(display_in_use, i, &mode) != 0)
			{
				SDL_Log("SDL_GetDisplayMode failed: %s", SDL_GetError());
				return 1;
			}
			// f = mode.format;
			modes[i] = mode;

			// SDL_Log("Mode %i\tbits per pixel %i\t%s\t%i x %i",
			// 		i, SDL_BITSPERPIXEL(f),
			// 		SDL_GetPixelFormatName(f),
			// 		mode.w, mode.h);
		}
		window_width = modes[0].w;
		window_height = modes[0].h;

		free(modes);
	}
	else
	{
		SDL_Log("SDL_GetNumDisplayModes failed: %s", SDL_GetError());
		return 1;
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
		}
	}
}

void draw_line(const int x0, const int y0, const int x1, const int y1, const uint32_t color)
{
	// DDA Alogorithm
	int delta_x = (x1 - x0);
	int delta_y = (y1 - y0);

	int side_lenght = (abs(delta_x) >= abs(delta_y)) ? abs(delta_x) : abs(delta_y);

	// Find how much we should increment in both x and y each step
	float x_inc = delta_x / (float)side_lenght;
	float y_inc = delta_y / (float)side_lenght;

	float current_x = x0;
	float current_y = y0;

	for (int i = 0; i <= side_lenght; i++)
	{
		draw_pixel(round(current_x), round(current_y), color);
		current_x += x_inc;
		current_y += y_inc;
	}
}

void draw_triangle(const int x0, const int y0, const int x1, const int y1, const int x2, const int y2, const uint32_t color)
{
	draw_line(x0, y0, x1, y1, color);
	draw_line(x1, y1, x2, y2, color);
	draw_line(x2, y2, x0, y0, color);
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
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
