#pragma once
#include <cglm/cglm.h>

typedef struct {
    vec3 Vertices;
    vec3 Normal;
    vec2 TexCoord;
} Vertex;

typedef struct {
    Vertex *data;
    size_t size;
    size_t count;
} Vertices;

typedef struct {
    unsigned int *data;
    size_t size;
    size_t count;
} Indices;

typedef struct {
    unsigned int id;
    char * type;
} Texture;

typedef struct {
    Texture *data;
    size_t size;
} Textures;

typedef struct {
    Vertices vertices;
    Indices indices;
    Textures textures;
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
} Mesh;

int parse_obj(FILE *file, Vertices *vertices, Indices *indices);
