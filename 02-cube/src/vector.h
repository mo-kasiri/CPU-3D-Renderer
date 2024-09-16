#ifndef VECTOR_H
#define VECTOR_H

typedef struct
{
    float x;
    float y;
} vec2_t;

typedef struct
{
    float x;
    float y;
    float z;
} vec3_t;

// TODO: Add functions to manipulate vectors
vec3_t vec3_rotate_x(const vec3_t v, const float angle);
vec3_t vec3_rotate_y(const vec3_t v, const float angle);
vec3_t vec3_rotate_z(const vec3_t v, const float angle);

#endif