#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

class Camera {
public:
    Camera();
    void processInput(GLFWwindow* window, int key, int action);
    glm::mat4 getViewMatrix();

private:
    glm::vec3 cameraPositions[3];
    int currentCameraPositionIndex;
    glm::mat4 viewMatrix;
    glm::vec3 cameraPosition;  //Position of the free look camera
    glm::vec3 cameraFront;     //Front vector of the free look camera
    glm::vec3 cameraUp;        //Up vector of the free look camera
    bool freeLook;             //Flag indicating whether the camera is in free look mode

    // Functions for free look camera movement
    void updateFreeLookCameraVectors();
    void processFreeLookInput(GLFWwindow* window, int key, int action);
    void processMouseMovement(GLFWwindow* window, double xpos, double ypos);

    // Constants for camera movement
    float cameraSpeed;
    float sensitivity;
};

#endif