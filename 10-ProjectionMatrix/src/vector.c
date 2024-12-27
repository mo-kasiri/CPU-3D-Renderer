#include <math.h>
#include "vector.h"
////////////////////////////////////////////
// 2D Vectors
///////////////////////////////////////////

float vec2_length(const vec2_t v)
{
    return (sqrt(pow(v.x, 2) + pow(v.y, 2)));
}

vec2_t vec2_add(const vec2_t v, const vec2_t w)
{
    vec2_t result = {
        .x = v.x + w.x,
        .y = v.y + w.y};
    return result;
};

vec2_t vec2_sub(const vec2_t v, const vec2_t w)
{
    vec2_t result = {
        .x = v.x - w.x,
        .y = v.y - w.y};
    return result;
};
vec2_t vec2_mul(const vec2_t v, const float factor)
{
    vec2_t result = {.x = v.x * factor, .y = v.x * factor};
    return result;
}

float vec2_dot(const vec2_t v, const vec2_t w)
{
    return (v.x * w.x + v.y * w.y);
}

void vec2_normalize(vec2_t *v)
{
    float length = sqrt(pow(v->x, 2) + pow(v->y, 2));
    v->x /= length;
    v->y /= length;
}

////////////////////////////////////////////////////
// 3D vectors
///////////////////////////////////////////////////

float vec3_length(const vec3_t v)
{
    return (sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2)));
};

vec3_t vec3_add(const vec3_t v, const vec3_t w)
{
    vec3_t result = {
        .x = v.x + w.x,
        .y = v.y + w.y,
        .z = v.z + w.z};
    return result;
};

vec3_t vec3_sub(const vec3_t v, const vec3_t w)
{
    vec3_t result = {
        .x = v.x - w.x,
        .y = v.y - w.y,
        .z = v.z - w.z};
    return result;
};

vec3_t vec3_mul(const vec3_t v, const float factor)
{
    vec3_t result = {.x = v.x * factor, .y = v.x * factor, .z = v.z * factor};
    return result;
}

vec3_t vec3_cross(const vec3_t v, const vec3_t w)
{
    vec3_t result = {
        .x = v.y * w.z - v.z * w.y,
        .y = v.z * w.x - v.x * w.z,
        .z = v.x * w.y - v.y * w.x};

    return result;
}

float vec3_dot(const vec3_t v, const vec3_t w)
{
    return (v.x * w.x + v.y * w.y + v.z * w.z);
}

void vec3_normalize(vec3_t *v)
{
    float length = sqrt(pow(v->x, 2) + pow(v->y, 2) + pow(v->z, 2));
    v->x /= length;
    v->y /= length;
    v->z /= length;
}

// Rotation Functions

vec3_t vec3_rotate_x(vec3_t v, float angle)
{
    vec3_t rotated_vector = {
        .x = v.x,
        .y = v.y * cos(angle) - v.z * sin(angle),
        .z = v.y * sin(angle) + v.z * cos(angle)};
    return rotated_vector;
}

vec3_t vec3_rotate_y(vec3_t v, float angle)
{
    vec3_t rotated_vector = {
        .x = v.x * cos(angle) - v.z * sin(angle),
        .y = v.y,
        .z = v.x * sin(angle) + v.z * cos(angle)};
    return rotated_vector;
}

vec3_t vec3_rotate_z(vec3_t v, float angle)
{
    vec3_t rotated_vector = {
        .x = v.x * cos(angle) - v.y * sin(angle),
        .y = v.x * sin(angle) + v.y * cos(angle),
        .z = v.z};
    return rotated_vector;
}

//////////////////////////////////////////////////////
// Vector 4
//////////////////////////////////////////////////////
vec4_t vec4_from_vec3(vec3_t v, float a)
{
    vec4_t vec4 = {.x = v.x, .y = v.y, .z = v.z, .w = a};
    return vec4;
}

vec3_t vec3_from_vec4(vec4_t v)
{
    vec3_t result = {.x = v.x, .y = v.y, .z = v.z};
    return result;
}