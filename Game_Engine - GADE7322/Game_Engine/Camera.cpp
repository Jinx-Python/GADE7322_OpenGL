#include "camera.h"
#include <iostream>

Camera::Camera() {
    cameraPositions[0] = glm::vec3(0.0f, 0.0f, 3.0f);   //position 1
    cameraPositions[1] = glm::vec3(0.0f, 3.0f, 0.0f);   //position 2
    cameraPositions[2] = glm::vec3(3.0f, 0.0f, 0.0f);   //position 3
    currentCameraPositionIndex = 0;
    viewMatrix = glm::lookAt(cameraPositions[currentCameraPositionIndex], glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);  //Initial position of the free look camera
    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    freeLook = false;
    pitch = 0.0f;  //Added initialization
    yaw = -90.0f;  //Added initialization
    cameraSpeed = 2.5f;       
    sensitivity = 0.1f;       
}

void Camera::processInput(GLFWwindow* window, int key, int action) {
    if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
        freeLook = !freeLook;
        if (!freeLook) {
            //If we are transitioning from free look to static position, reset the view matrix
            viewMatrix = glm::lookAt(cameraPositions[currentCameraPositionIndex], glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        }
    }

    if (freeLook) {
        processFreeLookInput(window, key, action);
    }
}

void Camera::processFreeLookInput(GLFWwindow* window, int key, int action) {
    if (key == GLFW_KEY_W) {
        cameraPosition += cameraSpeed * cameraFront;
    }
    else if (key == GLFW_KEY_S) {
        cameraPosition -= cameraSpeed * cameraFront;
    }
    else if (key == GLFW_KEY_A) {
        cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    else if (key == GLFW_KEY_D) {
        cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    else if (key == GLFW_KEY_LEFT) {
        currentCameraPositionIndex = (currentCameraPositionIndex - 1 + 3) % 3;
        freeLook = false;
        viewMatrix = glm::lookAt(cameraPositions[currentCameraPositionIndex], glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }
    else if (key == GLFW_KEY_RIGHT) {
        currentCameraPositionIndex = (currentCameraPositionIndex + 1) % 3;
        freeLook = false;
        viewMatrix = glm::lookAt(cameraPositions[currentCameraPositionIndex], glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    //Update view matrix
    viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
}

glm::mat4 Camera::getViewMatrix() {
    return viewMatrix;
}

void Camera::updateFreeLookCameraVectors() {
    //Update the camera's front vector based on mouse movement
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void Camera::processMouseMovement(GLFWwindow* window, double xpos, double ypos) {
    static double lastX = xpos;
    static double lastY = ypos;

    double xoffset = xpos - lastX;
    double yoffset = lastY - ypos;  //Reversed since y-coordinates range from bottom to top
    lastX = xpos;
    lastY = ypos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    //Constrain pitch to avoid camera flipping
    if (pitch > 89.0f) {
        pitch = 89.0f;
    }
    if (pitch < -89.0f) {
        pitch = -89.0f;
    }

    updateFreeLookCameraVectors();
}