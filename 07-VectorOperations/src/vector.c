#include <math.h>
#include "vector.h"

// 2D Vectors

//////////////////////////////////////////////
// Vector Length
//////////////////////////////////////////////
float vec2_length(const vec2_t v)
{
    return (sqrt(pow(v.x,2)+ pow(v.y,2)));
}
vec2_t vec2_add(const vec2_t v, const vec2_t w){
    vec2_t result = {
        .x = v.x + w.x,
        .y = v.y + w.y
    };
    return result;
};


// 3D vectors
float vec3_length(const vec3_t v)
{
    return (sqrt(pow(v.x,2) + pow(v.y,2) + pow(v.z,2)));
};

vec3_t vec3_add(const vec3_t v, const vec3_t w){
    vec3_t result = {
        .x = v.x + w.x,
        .y = v.y + w.y,
        .z = v.z + w.z
    };
    return result;
};


///////////////////////////////////////////////
/// Rotation Functions
///////////////////////////////////////////////
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