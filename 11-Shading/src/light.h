#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include "vector.h"

typedef struct
{
    vec3_t direction;
    float intensity;
} light_t;

extern light_t global_light;
extern light_t ambient_light;

uint32_t light_apply_intensity(const uint32_t original_color, float percentage_factor);