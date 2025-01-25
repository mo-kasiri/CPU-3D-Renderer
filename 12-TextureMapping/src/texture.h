#ifndef TEXTURE_H
#define TEXTURE_H
#include <stdint.h>

typedef struct // like vec2_t
{
    float u;
    float v;

} tex2_t;

extern unsigned int texture_width;
extern unsigned int texture_height;

extern const uint8_t REDBRICK_TEXTURE[];
extern uint32_t *mesh_texture;

void destroy_textures(void);

#endif