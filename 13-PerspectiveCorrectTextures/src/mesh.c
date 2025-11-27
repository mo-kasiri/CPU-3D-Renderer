#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "mesh.h"
#include "array.h"

#define LINE_BUFFER_SIZE 1024
// TODO: Create implementation for mesh.h functions

mesh_t mesh = {
    .vertices = NULL,
    .faces = NULL,
    .rotation = {0, 0, 0},
    .scale = {1.0, 1.0, 1.0},
    .translation = {0, 0, 0}};

vec3_t cube_vertices[N_CUBE_VERTICES] = {
    {.x = -1, .y = -1, .z = -1}, // 1
    {.x = -1, .y = 1, .z = -1},  // 2
    {.x = 1, .y = 1, .z = -1},   // 3
    {.x = 1, .y = -1, .z = -1},  // 4
    {.x = 1, .y = 1, .z = 1},    // 5
    {.x = 1, .y = -1, .z = 1},   // 6
    {.x = -1, .y = 1, .z = 1},   // 7
    {.x = -1, .y = -1, .z = 1}   // 8
};

face_t cube_faces[N_CUBE_FACES] = {
    // front
    {.a = 1, .b = 2, .c = 3, .a_uv = {0, 0}, .b_uv = {0, 1}, .c_uv = {1, 1}, .color = 0xFFFF0000},
    {.a = 1, .b = 3, .c = 4, .a_uv = {0, 0}, .b_uv = {1, 1}, .c_uv = {1, 0}, .color = 0xFFFF0000},

    // right
    {.a = 4, .b = 3, .c = 5, .a_uv = {0, 0}, .b_uv = {0, 1}, .c_uv = {1, 1}, .color = 0xFF00FFFF},
    {.a = 4, .b = 5, .c = 6, .a_uv = {0, 0}, .b_uv = {1, 1}, .c_uv = {1, 0}, .color = 0xFF00FFFF},

    // back
    {.a = 6, .b = 5, .c = 7, .a_uv = {0, 0}, .b_uv = {0, 1}, .c_uv = {1, 1}, .color = 0xFFFFFFFF},
    {.a = 6, .b = 7, .c = 8, .a_uv = {0, 0}, .b_uv = {1, 1}, .c_uv = {1, 0}, .color = 0xFFFFFFFF},

    // left
    {.a = 8, .b = 7, .c = 2, .a_uv = {0, 0}, .b_uv = {0, 1}, .c_uv = {1, 1}, .color = 0xFF00FF00},
    {.a = 8, .b = 2, .c = 1, .a_uv = {0, 0}, .b_uv = {1, 1}, .c_uv = {1, 0}, .color = 0xFF00FF00},

    // top
    {.a = 2, .b = 7, .c = 5, .a_uv = {0, 0}, .b_uv = {0, 1}, .c_uv = {1, 1}, .color = 0xFF0000FF},
    {.a = 2, .b = 5, .c = 3, .a_uv = {0, 0}, .b_uv = {1, 1}, .c_uv = {1, 0}, .color = 0xFF0000FF},

    // bottom
    {.a = 6, .b = 8, .c = 1, .a_uv = {0, 0}, .b_uv = {0, 1}, .c_uv = {1, 1}, .color = 0xFFFF00FF},
    {.a = 6, .b = 1, .c = 4, .a_uv = {0, 0}, .b_uv = {1, 1}, .c_uv = {1, 0}, .color = 0xFFFF00FF}

};

void load_cube_mesh_data(void)
{
    for (int i = 0; i < N_CUBE_VERTICES; i++)
    {
        vec3_t cube_vertex = cube_vertices[i];
        array_push(mesh.vertices, cube_vertex);
    }

    for (int j = 0; j < N_CUBE_FACES; j++)
    {
        face_t cube_face = cube_faces[j];
        array_push(mesh.faces, cube_face);
    }
    // mesh.rotation = {0.02, 0.02, 0.02};
};

void load_obj_file_data(const char *filename)
{
    // char abs_path[1024];
    // unsigned int varColor = 0xFFFF00FF;

    printf("Opening OBJ file: %s\n", filename);

    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Error opening file %s\n");
        return;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file))
    {
        // printf("LINE: %s", line);
        //  Vertex Information
        if (strncmp(line, "v ", 2) == 0)
        {
            vec3_t vertex;
            sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
            array_push(mesh.vertices, vertex);
        }

        // Face information
        if (strncmp(line, "f ", 2) == 0)
        {
            int vertex_indices[3];
            int texture_indices[3];
            int normal_indices[3];

            sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                   &vertex_indices[0], &texture_indices[0], &normal_indices[0],
                   &vertex_indices[1], &texture_indices[1], &normal_indices[1],
                   &vertex_indices[2], &texture_indices[2], &normal_indices[2]);

            face_t face = {
                .a = vertex_indices[0],
                .b = vertex_indices[1],
                .c = vertex_indices[2],
                .color = 0xFF999999};
            //.color = varColor};
            // varColor += 100;
            array_push(mesh.faces, face);
        }
    }
    printf("The Number of Vertices: %d\n", array_length(mesh.vertices));
    printf("The Number of Faces: %d\n", array_length(mesh.faces));

    if (fclose(file) != 0)
    {
        perror("Error closing file");
    }
}
