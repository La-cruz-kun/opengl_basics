#ifndef MESH_H
#define MESH_H
#include <glad/glad.h>
#include <cglm/cglm.h>
#include "../include/shader_s.h"
#include <stdio.h>
#include <string.h>

typedef struct {
    vec3 Vertices;
    vec3 Normal;
    vec2 TexCoord;
} Vertex;

typedef struct {
    Vertex *data;
    size_t size;
} Vertices;

typedef struct {
    unsigned int *data;
    size_t size;
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

static inline void setUpMesh(Mesh *mesh);

static inline void MeshInit(Mesh *mesh, Vertices vertices, Indices indices, Textures textures)
{
    mesh->vertices.data = (Vertex *) malloc(vertices.size * sizeof(Vertex));
    mesh->vertices.size = vertices.size;
    for (int i = 0; i < mesh->vertices.size; i++)
        mesh->vertices.data[i] = vertices.data[i];
    mesh->indices.data = (unsigned int *) malloc(indices.size * sizeof(unsigned int));
    mesh->indices.size = indices.size;
    for (int i = 0; i < mesh->indices.size; i++)
        mesh->indices.data[i] = indices.data[i];
    mesh->textures.data = (Texture *) malloc(textures.size * sizeof(Texture));
    mesh->textures.size = textures.size;
    for (int i = 0; i < mesh->textures.size; i++)
        mesh->textures.data[i] = textures.data[i];
    setUpMesh(mesh);
}

static inline void setUpMesh(Mesh *mesh)
{
    glGenVertexArrays(1, &(mesh->VAO));
    glGenBuffers(1, &mesh->VBO);
    glGenBuffers(1, &mesh->EBO);
    glBindVertexArray(mesh->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size * sizeof(Vertex),
    &mesh->vertices.data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size *
    sizeof(unsigned int), &mesh->indices.data[0], GL_STATIC_DRAW);
    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
    (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
    (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
    (void*)offsetof(Vertex, TexCoord));
    glBindVertexArray(0);
}

static inline void MeshDraw(Mesh mesh, Shader shader)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    char number[12];
    char uniformName[64];
    for (unsigned int i = 0; i < mesh.textures.size; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        char *name = mesh.textures.data->type;
        if (strcmp(name, "texture_diffuse") == 0)
            snprintf(number,sizeof(number), "%d", diffuseNr++);
        else if (strcmp(name, "texture_specular") == 0)
            snprintf(number, sizeof(number), "%d", specularNr++);
        snprintf(uniformName, sizeof(uniformName), "material.%s%s", name, number);
        ShaderSetFloat(shader, uniformName, i);
        glBindTexture(GL_TEXTURE_2D, mesh.textures.data[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(mesh.VAO);
    glDrawElements(GL_TRIANGLES, mesh.indices.size, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

#endif // !MESH_H
