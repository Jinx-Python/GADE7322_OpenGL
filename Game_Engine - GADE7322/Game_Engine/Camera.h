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
    void processKeyboardInput(int key, int action);
    glm::mat4 getViewMatrix();

private:
    glm::vec3 cameraPositions[3];
    int currentCameraPositionIndex;
    glm::mat4 viewMatrix;
};
#endif