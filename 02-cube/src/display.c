#include "display.h"
#include <SDL2/SDL_ttf.h>

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
uint32_t *color_buffer = NULL;
SDL_Texture *color_buffer_texture = NULL;
int window_width = 1920;
int window_height = 1080;

float delta_time = 0.f;
TTF_Font *font = NULL;

bool initilize_window(void)
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
		static int display_in_use = 0; /* Only using first display */

		int i, display_mode_count;
		SDL_DisplayMode mode;
		SDL_DisplayMode *modes = (SDL_DisplayMode *)malloc(sizeof(SDL_DisplayMode) * SDL_GetNumDisplayModes(display_in_use));
		Uint32 f;

		SDL_Log("SDL_GetNumVideoDisplays(): %i", SDL_GetNumVideoDisplays());

		display_mode_count = SDL_GetNumDisplayModes(display_in_use);

		SDL_Log("SDL_GetNumDisplayModes: %i", display_mode_count);

		for (i = 0; i < display_mode_count; ++i)
		{
			if (SDL_GetDisplayMode(display_in_use, i, &mode) != 0)
			{
				SDL_Log("SDL_GetDisplayMode failed: %s", SDL_GetError());
				return 1;
			}
			f = mode.format;
			modes[i] = mode;

			SDL_Log("Mode %i\tbits per pixel %i\t%s\t%i x %i",
					i, SDL_BITSPERPIXEL(f),
					SDL_GetPixelFormatName(f),
					mode.w, mode.h);
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

void load_font(void)
{
	TTF_Init();
	font = TTF_OpenFont("Ubuntu-B.ttf", 15);
	if (font == NULL)
		printf("Font loading faild: %s\n", TTF_GetError());
}

void write_text(void)
{
	char buffer[32];
	snprintf(buffer, sizeof(buffer), "Delta Time: %.4f", delta_time);
	SDL_Surface *TextSurface = TTF_RenderText_Blended_Wrapped(font, buffer, (SDL_Color){255, 255, 255, 255}, 500);
	if (TextSurface == NULL)
		printf("Surface creation faild: %s\n", TTF_GetError());

	SDL_Texture *TextTexture = SDL_CreateTextureFromSurface(renderer, TextSurface);
	if (TextTexture == NULL)
		printf("Texture creation faild: %s\n", TTF_GetError());

	SDL_FreeSurface(TextSurface);

	SDL_RenderCopy(
		renderer,
		TextTexture,
		NULL,
		&(SDL_Rect){10, 10, TextSurface->w, TextSurface->h}); // Creating an SDL_Rect
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
