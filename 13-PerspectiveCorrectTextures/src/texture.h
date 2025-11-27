#ifndef TEXTURE_H
#define TEXTURE_H
#include <stdint.h>
#include <stdio.h>

typedef struct // like vec2_t
{
    float u;
    float v;

} tex2_t;

extern unsigned int texture_width;
extern unsigned int texture_height;

extern const uint8_t REDBRICK_TEXTURE[];
extern const uint32_t BRICK_TEXTURE_32[];
extern uint32_t *mesh_texture;

uint32_t rgba_to_argb(uint32_t rgba);

#endif