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
    vec3 cubePosition[] = {{0.0f, 0.0f, 0.0f}, {-3.0f, 0.0f, 0.0f}, {3.0f, 0.0f, 0.0f}, {6.0f, 0.0f, 0.0f}, {-6.0f, 0.0f, 0.0f}};
    vec3 lightPosition = {0.f, 2.0f, 0.0f};
    Material material[] = {
        {{0.0, 0.1, 0.06}, {0.0, 0.50980392, 0.50980392}, {0.50196078, 0.50196078, 0.50196078}, 0.25},
        {{0.2125, 0.1275, 0.054}, {0.780392, 0.568627, 0.113725}, {0.992157, 0.941176, 0.807843}, 0.21794872},
        {{0.05375, 0.05, 0.06625}, {0.18275, 0.17, 0.22525}, {0.332741, 0.328634, 0.346435}, 0.3},
        {{0.19225, 0.19225, 0.19225}, {0.50754, 0.50754, 0.50754}, {0.508273, 0.508273, 0.508273}, 0.4},
        {{0.24725, 0.1995, 0.0745}, {0.75164, 0.60648, 0.22648}, {0.628281, 0.555802, 0.366065}, 0.4}
    };
    Shader shader = {"material/vertex.glsl", "material/fragment.glsl", 0};
    Shader lightShader = {"material/light_vertex.glsl", "material/light_fragment.glsl", 0};
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

    mat4 modelCube = GLM_MAT4_IDENTITY;
    mat3 normalModel = GLM_MAT3_IDENTITY;
    mat4 projection = GLM_MAT4_IDENTITY;
    glm_perspective(glm_rad(45.0), (float)WINDOWWIDTH / (float)WINDOWHEIGHT, 0.1f,
                  100.0f, projection);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    vec3 lightColor = {1, 1, 1};
    ShaderUse(shader);
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);

        vec3 lightdiffuse;
        glm_vec3_mul((vec3) {0.5, 0.5, 0.5}, lightColor, lightdiffuse);
        vec3 lightambient;
        glm_vec3_mul((vec3) {0.2, 0.2, 0.2}, lightColor, lightambient);

        lightPosition[0] = 4 * sin(glfwGetTime());
        lightPosition[2] = 4 * cos(glfwGetTime());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0, 0, 0, 1.0);


        //cyan plastic
        ShaderUse(shader);
        GLuint matAmbient = glGetUniformLocation(shader.ID, "material.ambient");
        GLuint matDiffuse = glGetUniformLocation(shader.ID, "material.diffuse");
        GLuint matSpecular = glGetUniformLocation(shader.ID, "material.specular");
        GLuint matShininess = glGetUniformLocation(shader.ID, "material.shininess");
        // get uniforms
        glUniform3f(glGetUniformLocation(shader.ID, "alightPos"), lightPosition[0], lightPosition[1], lightPosition[2]);

        glUniform3f(glGetUniformLocation(shader.ID, "light.ambient"), lightambient[0], lightambient[1], lightambient[2]);
        glUniform3f(glGetUniformLocation(shader.ID, "light.diffuse"), lightdiffuse[0], lightdiffuse[1], lightdiffuse[2]);
        glUniform3f(glGetUniformLocation(shader.ID, "light.specular"), 1.0, 1.0, 1.0);
        mat4 view;
        GetViewMatrix(&camera, view);
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE,
                           &view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1,
                           GL_FALSE, &projection[0][0]);
        glBindVertexArray(VAO);
        for (int i = 0; i < 5; i++) {
            glUniform3f(matAmbient, material[i].ambient[0], material[i].ambient[1], material[i].ambient[2]);
            glUniform3f(matDiffuse, material[i].diffuse[0], material[i].diffuse[1], material[i].diffuse[2]);
            glUniform3f(matSpecular, material[i].specular[0], material[i].specular[1], material[i].specular[2]);
            glUniform1f(matShininess, material[i].shininess);
            glm_mat4_identity(modelCube);
            glm_translate(modelCube, cubePosition[i]);
            glm_mat3_inv((mat3) {{modelCube[0][0], modelCube[0][1], modelCube[0][2]}, 
                    {modelCube[1][0], modelCube[1][1], modelCube[1][2]}, 
                    {modelCube[2][0], modelCube[2][1], modelCube[2][2]}}, normalModel);
            glUniformMatrix3fv(glGetUniformLocation(shader.ID, "normalModel"), 1, GL_FALSE,
                     &normalModel[0][0]);
            glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE,
                     &modelCube[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        
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
