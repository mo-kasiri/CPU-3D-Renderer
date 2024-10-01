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

//////////////////////////////////////////////////////
// Vector 2D functions 
//////////////////////////////////////////////////////

float vec2_length(const vec2_t v);
vec2_t vec2_add(const vec2_t v, const vec2_t w);
vec2_t vec2_sub(const vec2_t v, const vec2_t w);
vec2_t vec2_mul(const vec2_t v, const float factor);
float vec2_dot(const vec2_t v, const vec2_t w);


//////////////////////////////////////////////////////
// Vector 3D functions 
//////////////////////////////////////////////////////

float vec3_length(const vec3_t v);
vec3_t vec3_add(const vec3_t v, const vec3_t w);
vec3_t vec3_sub(const vec3_t v, const vec3_t w);
vec2_t vec2_mul(const vec2_t v, const float factor);
vec3_t vec3_cross(const vec3_t v, const vec3_t w);
float vec3_dot(const vec3_t v, const vec3_t w);

vec3_t vec3_rotate_x(vec3_t v, float angle);
vec3_t vec3_rotate_y(vec3_t v, float angle);
vec3_t vec3_rotate_z(vec3_t v, float angle);

#endif