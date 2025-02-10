#pragma once

#include <glad/glad.h>
#include <cglm/cglm.h>

typedef enum { FORWARD, BACKWARD, LEFT, RIGHT } Camera_Movement;

const float YAW = -90.0;
const float PITCH = 0.0;
const float SPEED = 2.5;
const float SENSITIVITY = 0.1;
const float ZOOM = 45.0;

typedef struct {
  vec3 Position;
  vec3 Front;
  vec3 Up;
  vec3 Right;
  vec3 WorldUp;

  float Yaw;
  float Pitch;
  float MovementSpeed;
  float MouseSensitivity;
  float Zoom;

} Camera;

void CameraInitVector(Camera camera, vec3 position, vec3 up, float yaw, float pitch);

void CameraInitScaler(Camera camera, float posX, float posY, float posZ, float upX, float upY,
                      float upZ, float yaw, float pitch);

void CameraGetViewMatrix(Camera camera, mat4 destination);

void CameraProcessKeyboard(Camera camera, Camera_Movement direction, float deltaTime);

void CameraProcessMouseMovement(Camera camera, float xoffset, float yoffset,
                                GLboolean constrainPitch);

void CameraProcessMouseScroll(Camera camera, float yoffset);

void updateCameraVector(Camera camera);
