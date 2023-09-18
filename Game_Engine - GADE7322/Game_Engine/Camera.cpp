#include "camera.h"

Camera::Camera() {
    cameraPositions[0] = glm::vec3(0.0f, 0.0f, 3.0f);   //position 1
    cameraPositions[1] = glm::vec3(0.0f, 3.0f, 0.0f);   //position 2
    cameraPositions[2] = glm::vec3(3.0f, 0.0f, 0.0f);   //position 3
    currentCameraPositionIndex = 0;
    viewMatrix = glm::lookAt(cameraPositions[currentCameraPositionIndex], glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::processKeyboardInput(int key, int action) {
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
        currentCameraPositionIndex = (currentCameraPositionIndex + 1) % 3;
    }
    else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
        currentCameraPositionIndex = (currentCameraPositionIndex - 1 + 3) % 3;
    }
    viewMatrix = glm::lookAt(cameraPositions[currentCameraPositionIndex], glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 Camera::getViewMatrix() {
    return viewMatrix;
}