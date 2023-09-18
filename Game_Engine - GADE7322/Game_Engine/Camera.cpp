#include"Camera.h"

Camera::Camera()
    : position(glm::vec3(0.0f, 0.0f, 3.0f)),
    front(glm::vec3(0.0f, 0.0f, -1.0f)),
    up(glm::vec3(0.0f, 1.0f, 0.0f)),
    worldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
    yaw(-90.0f),
    pitch(0.0f),
    movementSpeed(2.5f),
    sensitivity(0.1f) {
    updateCameraVectors();

}

void Camera::init(glm::vec3 initialPosition, glm::vec3 target, glm::vec3 upVector) {
    position = initialPosition;
    front = glm::normalize(target - position);
    up = upVector;
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::getProjectionMatrix() const {
    return projection;
}

void Camera::processInput(GLFWwindow* window) {
    float deltaTime = 0.1f;
    //handle input to change camera positions

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        yaw += sensitivity * deltaTime; 
        updateCameraVectors();
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        yaw -= sensitivity * deltaTime; 
        updateCameraVectors();
    }
}

void Camera::updateCameraVectors() {
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}