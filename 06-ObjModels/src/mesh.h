#ifndef MESH_H
#define MESH_H

#include "vector.h"
#include "triangle.h"
#include "array.h"

#define N_CUBE_VERTICES 8
#define N_CUBE_FACES (6 * 2) // 6 cube faces, 2 triangles per face

extern vec3_t cube_vertices[N_CUBE_VERTICES];
extern face_t cube_faces[N_CUBE_FACES];

///////////////////////////////////////////////////////////////////////////////////////
// Defince a struct for dynamic size meshes, with dynamic array of vertices and faces
///////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
    vec3_t *vertices; // Dynamic array of vertices
    face_t *faces;    // Dynamic array of faces
    vec3_t rotation;  // Rotation with x,y and z values
} mesh_t;

extern mesh_t mesh;

void load_cube_mesh_data(void);
void load_obj_file_data(const char *filename);

#endif