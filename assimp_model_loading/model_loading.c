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

int WINDOW_HIGHT = 720;
int WINDOW_WIDTH = 1200;

Camera camera;
bool firstMouse = true;
float lastX = 600;
float lastY = 360;
float deltaTime = 0.0;
float lastFrame = 0.0;

int main() {
    Camera_init(&camera, (vec3){0.0, 0.0, 3.0}, (vec3){0.0, 1.0, 0.0},  90.0, 0.0);
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

    FILE *obj;
    Vertices cube_vertices;
    Indices cube_indices;
    if (!(obj = fopen("resource/cube.obj", "r"))){
        printf("couldn't open 'cube.obj'\n");
    }
    parse_obj(obj, &cube_vertices, &cube_indices);

  Shader shader = {
      "assimp_model_loading/shader.vs",
      "assimp_model_loading/shader.fs",
  };
  ShaderInit(&shader);

  unsigned int VBO;
  unsigned int VAO;
  unsigned int EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO); // Generate the Element Buffer Object

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, cube_vertices.size * sizeof(Vertex), cube_vertices.data, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cube_indices.size * sizeof(unsigned int), cube_indices.data, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Vertices));
  glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Note: EBO binding is part of VAO state

  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  int width;
  int height;
  int nrChannel;
  unsigned char *data =
      stbi_load("container.jpg", &width, &height, &nrChannel, 0);

  if (data) { // Generate texture ID
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    printf("couldn't load image\n");
    glfwTerminate();
    return -1;
  };

  stbi_image_free(data);
  ShaderUse(shader);


  while (!glfwWindowShouldClose(window)) {
    // input
    float currentFrame = (float) glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindTexture(GL_TEXTURE_2D, texture);

    ShaderUse(shader);
      mat4 projection = GLM_MAT4_IDENTITY;
  glm_perspective(camera.Zoom, (float)WINDOW_WIDTH / (float)WINDOW_HIGHT,
                  0.1f, 100.f, projection);
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1,
                       GL_FALSE, &projection[0][0]);
    mat4 view = GLM_MAT4_IDENTITY;
    GetViewMatrix(&camera, view);
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE,
                       &view[0][0]);

    glBindVertexArray(VAO);
    mat4 model = GLM_MAT4_IDENTITY;
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE,
                 &model[0][0]);
    glDrawElements(GL_TRIANGLES, cube_indices.size, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
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
