#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "game.h"

// Default camera values
const float YAW         =  60.0f;
const float PITCH       =  -20.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;

    // constructors
    Camera(glm::vec3 position = glm::vec3(0.0f, 6.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

/*    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);*/

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix();

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Direction direction, float deltaTime);

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();
};
#endif
