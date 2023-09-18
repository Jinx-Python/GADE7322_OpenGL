#pragma once
#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include"shader.h"

class Camera {
public:
    Camera();

    void init(glm::vec3 initialPosition, glm::vec3 target, glm::vec3 up);
    void update(float deltaTime);
    glm::mat4 getViewMatrix() const;
    glm::mat4 projection;
    void switchToCameraPosition(int positionIndex);
    void processInput(GLFWwindow* window);
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    void setCameraPosition(glm::vec3 position, glm::vec3 target);
    void setProjection(const glm::mat4& projMatrix) {
        projection = projMatrix;
    }
    glm::mat4 getProjectionMatrix() const;


private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;

    float movementSpeed;
    float sensitivity;

    void updateCameraVectors();

    //define three camera positions
    glm::vec3 cameraPositions[3] = {
        glm::vec3(0.0f, 0.0f, 3.0f),   // Position 0
        glm::vec3(3.0f, 0.0f, 0.0f),   // Position 1
        glm::vec3(0.0f, 3.0f, 0.0f)    // Position 2
    };
};

#endif // !CAMERA_CLASS_H
