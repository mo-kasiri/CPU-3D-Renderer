#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

bool is_running = false;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

SDL_Texture *color_buffer_texture = NULL;
uint32_t *color_buffer = NULL;

unsigned int window_width = 800;
unsigned int window_height = 600;

bool initialize_window(void)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "Error initializing SDL.\n");
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

    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        window_width,
        window_height,
        SDL_WINDOW_BORDERLESS);

    if (!window)
    {
        fprintf(stderr, "Error creating SDL Window.\n");
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer)
    {
        fprintf(stderr, "Error creating SDL renderer.\n");
        return false;
    }
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

    return true;
}

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

void draw_grid(const unsigned int size)
{
    for (int i = 0; i < window_width; i++)
    {
        for (int j = 0; j < window_height; j++)
        {
            if (i % size == 0 || i % size == 1 || j % size == 0 || j % size == 1)
                color_buffer[window_width * j + i] = 0xFF555555;
        }
    }
}

void draw_rectangle(const int x, const int y, const unsigned int width, const unsigned int height, const u_int32_t color)
{
    for (int i = 0; i <= height; i++)
    {
        for (int j = 0; j <= width; j++)
        {
            color_buffer[(i + y) * window_width + (j + x)] = color;
        }
    }
}

void clear_color_buffer(uint32_t color)
{
    for (int y = 0; y < window_height; y++)
    {
        for (int x = 0; x < window_width; x++)
        {
            color_buffer[window_width * y + x] = color;
        }
    }
}

void render(void)
{
    // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderClear(renderer);

    clear_color_buffer(0xFFFFFF00);

    draw_grid(100);
    draw_rectangle(100, 200, 100, 200, 0xFFFF0000);

    render_color_buffer();
    SDL_RenderPresent(renderer);
}

void destroy_window(void)
{
    free(color_buffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
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
