#ifndef MODEL_H
#define MODEL_H

#include "mesh.h"
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

typedef struct {
    Mesh *data;
    size_t size;
} Meshes;

typedef struct {
    Meshes meshes;
    char *directory;
} Model;

static inline void ModelInit(Model *model, char *path);
static inline void loadModel(Model *model, char *path);
static inline void processNode(aiNode *node, aiScene *scene);


static inline void ModelInit(Model *model, char *path)
{
    loadModel(model, path);
}

static inline void ModelDraw(Model model, Shader shader)
{
    for (size_t i = 0; i < model.meshes.size; i++) {
        MeshDraw(model.meshes.data[i], shader);
    }
}

static inline void loadModel(Model *model, char *path)
{
    const struct aiScene* scene = aiImportFile(
            path,
            aiProcess_Triangulate | aiProcess_FlipUVs
        );
    if (!scene || 
            (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || 
            !scene->mRootNode) 
        {
            fprintf(stderr, "ERROR::ASSIMP::%s\n", aiGetErrorString());
            return;
        }

        // Extract directory from path
        const char* last_slash = strrchr(path, '/');
        if (last_slash) {
            size_t dir_length = last_slash - path;
            char* dir = (char*)malloc(dir_length + 1);
            strncpy(dir, path, dir_length);
            dir[dir_length] = '\0';
            model->directory = dir;  // Would need proper memory management
        } else {
            model->directory = ".";  // Default to current directory
        }

        // Process nodes
        processNode(scene->mRootNode, scene);

        // Cleanup
        aiReleaseImport(scene);
}
#endif // !MODEL_H
