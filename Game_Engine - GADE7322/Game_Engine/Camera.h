#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
public:
    Camera();
    void processInput(GLFWwindow* window, int key, int action);
    glm::mat4 getViewMatrix();
    void processMouseMovement(GLFWwindow* window, double xpos, double ypos);

private:
    glm::vec3 cameraPositions[3];
    int currentCameraPositionIndex;
    glm::vec3 cameraPosition;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    glm::mat4 viewMatrix;

    //Free Look Camera Variables
    bool freeLook;
    float pitch;
    float yaw;
    float cameraSpeed;
    float sensitivity;

    void processFreeLookInput(GLFWwindow* window, int key, int action);
    void updateFreeLookCameraVectors();
};

#endif 