#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <cglm/affine.h>
#include <cglm/cglm.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/camera.h"
#include "../include/shader_s.h"
#include "../include/model.h"
#include "../include/stb_image.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn);
unsigned int loadTexture(char const * path);
unsigned int create_vao_vbo(Vertices vertices, Indices indices);

int WINDOW_HIGHT = 720;
int WINDOW_WIDTH = 1200;

Camera camera;
bool firstMouse = true;
float lastX = 600;
float lastY = 360;
float deltaTime = 0.0;
float lastFrame = 0.0;

int main() {
    Camera_init(&camera, (vec3){0.0, 0.0, -3.0}, (vec3){0.0, 1.0, 0.0},  90.0, 0.0);
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window =
      glfwCreateWindow(WINDOW_WIDTH, WINDOW_HIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
    printf("failed to create a GLFW window\n");
    glfwTerminate();
    return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("Failed to initialize GLAD\n");
    return -1;
    }
    glEnable(GL_DEPTH_TEST);

    char *cube_loc = "resource/cube.obj";
    Vertices cube_vertices;
    Indices cube_indices;
    Texture cube_tex;
    parse_obj(cube_loc, &cube_vertices, &cube_indices, &cube_tex);


    char *obj = "resource/cube.obj";
    Vertices obj_vertices;
    Indices obj_indices;
    Texture obj_tex;
    parse_obj(obj, &obj_vertices, &obj_indices, &obj_tex);


    Shader light_shader = {
      "assimp_model_loading/shader.vs",
      "assimp_model_loading/shader.fs",
      0
    };

    Shader models_shader = {
      "assimp_model_loading/vertex.glsl",
      "assimp_model_loading/fragment.glsl",
      0
    };

    ShaderInit(&light_shader);
    ShaderInit(&models_shader);

    unsigned int lightVAO = create_vao_vbo(cube_vertices, cube_indices);
    unsigned int modelVAO = create_vao_vbo(obj_vertices, obj_indices);
    vec3 lightPos = {2, 4, 0};

    unsigned int container = loadTexture("container.jpg");


    mat4 projection = GLM_MAT4_IDENTITY;
    mat4 view = GLM_MAT4_IDENTITY;
    mat4 model = GLM_MAT4_IDENTITY;
    glm_perspective(camera.Zoom, (float)WINDOW_WIDTH / (float)WINDOW_HIGHT,
                    0.1f, 10000.f, projection);
    ShaderUse(models_shader);
    glUniform3f(glGetUniformLocation(models_shader.ID, "light.ambient"), 0.2, 0.2, 0.2);
    glUniform3f(glGetUniformLocation(models_shader.ID, "light.position"), lightPos[0], lightPos[1], lightPos[2]);
    glUniform3f(glGetUniformLocation(models_shader.ID, "light.diffuse"), 0.5, 0.5, 0.5);
    glUniform3f(glGetUniformLocation(models_shader.ID, "light.specular"), 1.0, 1.0, 1.0);
    glUniformMatrix4fv(glGetUniformLocation(models_shader.ID, "projection"), 1,
                       GL_FALSE, &projection[0][0]);



    while (!glfwWindowShouldClose(window)) {
        // input
        float currentFrame = (float) glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render model
        glBindTexture(GL_TEXTURE_2D, container);

        ShaderUse(models_shader);
        GetViewMatrix(&camera, view);
        glUniform3f(glGetUniformLocation(models_shader.ID, "material.diffuse"), obj_tex.diffuse[0], obj_tex.diffuse[1], obj_tex.diffuse[2]);
        glUniform3f(glGetUniformLocation(models_shader.ID, "material.specular"), obj_tex.specular[0], obj_tex.specular[1], obj_tex.specular[2]);
        glUniform1f(glGetUniformLocation(models_shader.ID, "material.shininess"), obj_tex.shininess);
        glUniformMatrix4fv(glGetUniformLocation(models_shader.ID, "view"), 1, GL_FALSE,
                           &view[0][0]);
        glBindVertexArray(modelVAO);
        glUniformMatrix4fv(glGetUniformLocation(models_shader.ID, "model"), 1, GL_FALSE,
                     &model[0][0]);
        glDrawElements(GL_TRIANGLES, obj_indices.size, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    ProcessKeyboard(&camera, UP, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    ProcessKeyboard(&camera, DOWN, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    ProcessKeyboard(&camera, FORWARD, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    ProcessKeyboard(&camera, BACKWARD, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
    ProcessKeyboard(&camera, LEFT, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    ProcessKeyboard(&camera, RIGHT, deltaTime);
  }
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
    float xpos = (float) xposIn;
    float ypos = (float) yposIn;
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;
  lastX = xpos;
  lastY = ypos;
  ProcessMouseMovement(&camera, xoffset, yoffset, true);
}


unsigned int create_vao_vbo(Vertices vertices, Indices indices)
{
    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO); // Generate the Element Buffer Object

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size * sizeof(Vertex), vertices.data, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size * sizeof(unsigned int), indices.data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Vertices));
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Note: EBO binding is part of VAO state
    return VAO;
}



unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format = 0;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(data);
    }
    else
    {
        printf("Texture failed to load at path: %s\n", path);
        stbi_image_free(data);
    }

    return textureID;
}
