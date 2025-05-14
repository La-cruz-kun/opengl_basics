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
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
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

int parse_obj(char *path, Vertices *vertices, Indices *indices, Texture *texture);
int parse_mtl(char *path, Texture *texture);
