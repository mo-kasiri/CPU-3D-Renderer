#include "light.h"

light_t global_light = {.direction = {.x = 0.0, .y = 1.0, .z = 1.0}, .intensity = 1.0f};
light_t ambient_light = {.direction = {0}, .intensity = 0.3f};

uint32_t light_apply_intensity(const uint32_t original_color, float percentage_factor)
{
    // Clamping between 0 and 1
    percentage_factor = percentage_factor >= 1 ? 1 : percentage_factor;
    // Adding ambient light
    percentage_factor = percentage_factor < ambient_light.intensity ? ambient_light.intensity : percentage_factor;

    uint32_t a = (original_color & 0xFF000000);                     // Extract alpha part
    uint32_t r = (original_color & 0x00FF0000) * percentage_factor; // Extract red part
    uint32_t g = (original_color & 0x0000FF00) * percentage_factor; // Extract green part
    uint32_t b = (original_color & 0x000000FF) * percentage_factor; // Extract blue part

    uint32_t new_color = a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF); // Combine a r g b to create new color

    return new_color;
}