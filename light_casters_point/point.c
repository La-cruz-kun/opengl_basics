#include <cglm/affine.h>
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/shader_s.h"
#include "../include/stb_image.h"
#include "../include/camera.h"


void framebuffer_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
unsigned int loadTexture(char const * path);

int WINDOWWIDTH = 1280;
int WINDOWHEIGHT = 720;
float PlaneScale = 100.0;
float deltaTime = 0.0;
float lastFrame = 0.0;

Camera camera;
float yaw = -117.0;
float pitch = -14.0;

bool firstMouse = true;
float lastX = 640;
float lastY = 360;

vec3 result;
vec3 temp;

typedef struct {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
} Material;

int main() {
    Camera_init(&camera, (vec3) {7, 4, 11}, (vec3) {0, 1, 0}, yaw, pitch);
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    GLFWwindow *window = glfwCreateWindow(WINDOWWIDTH, WINDOWHEIGHT, "Test Engine", NULL, NULL);
    if (window == NULL) {
        printf("unable to create window\n");
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("unable to initialize glad\n");
        glfwTerminate();
    }

    glEnable(GL_DEPTH_TEST);
    float cubeVertices[] = {
        // Positions          // Normals           // Texture Coords
        // Front Face (Z = 1)
        -1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,  // Bottom-left
         1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,  // Bottom-right
         1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,  // Top-right
         1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,  // Top-right
        -1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,  // Top-left
        -1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,  // Bottom-left

        // Back Face (Z = -1)
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // Bottom-left
         1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,  // Bottom-right
         1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,  // Top-right
         1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,  // Top-right
        -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,  // Top-left
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // Bottom-left

        // Left Face (X = -1)
        -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // Bottom-back
        -1.0f, -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // Bottom-front
        -1.0f,  1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // Top-front
        -1.0f,  1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // Top-front
        -1.0f,  1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // Top-back
        -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // Bottom-back

        // Right Face (X = 1)
         1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // Bottom-back
         1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // Bottom-front
         1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // Top-front
         1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // Top-front
         1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // Top-back
         1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // Bottom-back

        // Top Face (Y = 1)
        -1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // Back-left
         1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,  // Back-right
         1.0f,  1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // Front-right
         1.0f,  1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // Front-right
        -1.0f,  1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,  // Front-left
        -1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // Back-left

        // Bottom Face (Y = -1)
        -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,  // Back-left
         1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,  // Back-right
         1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,  // Front-right
         1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,  // Front-right
        -1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  // Front-left
        -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,  // Back-left
    };
// Interleaved data: [POS.X, POS.Y, POS.Z, NORM.X, NORM.Y, NORM.Z, TEX.U, TEX.V]
    float planeVertices[] = {
        // First triangle
        -1.0f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,  // Bottom-left
         1.0f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f,  10.0f, 0.0f,  // Bottom-right
         1.0f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f,  // Top-right

        // Second triangle
         1.0f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f,  // Top-right
        -1.0f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 10.0f,  // Top-left
        -1.0f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f   // Bottom-left
    };

  vec3 cubePositions[] = {
      {5.0f, 5.0f, -15.0f}, {-1.5f, -2.2f, -2.5f}, 
      {-3.8f, -2.0f, -12.3f}, {2.4f, -0.4f, -3.5f},  
      {-4.7f, 3.0f, -7.5f}, {3.3f, -2.0f, 0},  
      {1.5f, 2.0f, 0}, {1.5f, 0.2f, 3},  
      {0.0f, 0.0f, 0.0f},   {6.3f, 1.0f, -1.5f}};
    vec3 lightPosition = {0.f, 2.0f, 0.0f};
    Shader shader = {"light_casters_point/vertex.glsl", "light_casters_point/fragment.glsl", 0};
    Shader lightShader = {"light_casters_point/light_vertex.glsl", "light_casters_point/light_fragment.glsl", 0};
    ShaderInit(&shader);
    ShaderInit(&lightShader);
    unsigned int VAO;
    unsigned int VBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices,
               GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6* sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int lightVAO;

    glGenVertexArrays(1, &lightVAO);

    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices,
               GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    unsigned int planeVAO;

    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices,
               GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6* sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned char diffuse_data = loadTexture("container2.png");
    unsigned char specular_data = loadTexture("container2_specular.png");
    unsigned char image_data = loadTexture("wall.jpg");
    mat4 modelCube = GLM_MAT4_IDENTITY;
    mat4 projection = GLM_MAT4_IDENTITY;
    glm_perspective(glm_rad(45.0), (float)WINDOWWIDTH / (float)WINDOWHEIGHT, 0.1f,
                  100.0f, projection);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    vec3 lightColor = {1, 1, 1};
    ShaderUse(shader);
    glUniform1i(glGetUniformLocation(shader.ID, "material.diffuse"), 0);
    glUniform1i(glGetUniformLocation(shader.ID, "material.specular"), 1);
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0, 0, 0, 1.0);


        //cyan plastic
        ShaderUse(shader);
        // get uniforms

        glUniform3f(glGetUniformLocation(shader.ID, "viewPos"), camera.Position[0], camera.Position[1], camera.Position[2]);
        glUniform3f(glGetUniformLocation(shader.ID, "light.position"), lightPosition[0], lightPosition[1], lightPosition[2]);
        glUniform1f(glGetUniformLocation(shader.ID, "light.constant"), 1);
        glUniform1f(glGetUniformLocation(shader.ID, "light.linear"), 0.09);
        glUniform1f(glGetUniformLocation(shader.ID, "light.quadratic"), 0.032);

        glUniform3f(glGetUniformLocation(shader.ID, "light.ambient"), 0.2, 0.2, 0.2);
        glUniform3f(glGetUniformLocation(shader.ID, "light.diffuse"), 0.5, 0.5, 0.5);
        glUniform3f(glGetUniformLocation(shader.ID, "light.specular"), 1.0, 1.0, 1.0);
        glUniform3f(glGetUniformLocation(shader.ID, "material.specular"), 0.5, 0.5, 0.5);
        glUniform1f(glGetUniformLocation(shader.ID, "material.shininess"), 64);
        mat4 view;
        GetViewMatrix(&camera, view);
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE,
                           &view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1,
                           GL_FALSE, &projection[0][0]);
        for (unsigned int i = 0; i < 10; i++) {
          mat4 model = GLM_MAT4_IDENTITY;
          glm_translate(model, cubePositions[i]);

          glm_scale(model, (vec3) {0.8, 0.8, 0.8});
          float angle = 20.f * i;
          if (i % 3 == 0) {
            glm_rotate(model, (float)glfwGetTime() * glm_rad(angle),
                       (vec3){1.0, 0.3, 0.5});
            glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1,
                               GL_FALSE, &model[0][0]);
          }
          if (i == 0) {
            angle = 20.f * (5 - i);
            glm_rotate(model, (float)glfwGetTime() * glm_rad(angle),
                       (vec3){1.0, 0.3, 0.5});
            glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1,
                               GL_FALSE, &model[0][0]);
          }
          glm_rotate(model, glm_rad(angle), (vec3){1.0, 0.3, 0.5});
          glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE,
                             &model[0][0]);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuse_data);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specular_data);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        ShaderUse(shader);
        glm_mat4_identity(modelCube);
        glm_translate(modelCube, (vec3) {0, -3.5, 0});
        glm_scale(modelCube, (vec3) {20, 1, 20});
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE,
                 &modelCube[0][0]);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, image_data);
        glBindVertexArray(planeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // render light source
        ShaderUse(lightShader);
        GetViewMatrix(&camera, view);
        glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "view"), 1, GL_FALSE,
                           &view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "projection"), 1,
                           GL_FALSE, &projection[0][0]);
        glUniform3f(glGetUniformLocation(lightShader.ID, "Color"), lightColor[0], lightColor[1], lightColor[2]);

        glm_mat4_identity(modelCube);
        glm_translate(modelCube, lightPosition);
        glm_scale(modelCube, (vec3) {0.1, 0.1, 0.1});
        glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE,
                 &modelCube[0][0]);
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if ((glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)) {
        ProcessKeyboard(&camera, UP, deltaTime);
    }
    if ((glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)) {
        ProcessKeyboard(&camera, DOWN, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        ProcessKeyboard(&camera, FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        ProcessKeyboard(&camera, BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        ProcessKeyboard(&camera, RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        ProcessKeyboard(&camera, LEFT, deltaTime);
    }
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
    float xpos = xposIn;
    float ypos = yposIn;
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

void framebuffer_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
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

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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
