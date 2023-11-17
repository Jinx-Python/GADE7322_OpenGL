#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include <vector>
#include "basicMesh.h"
#include <filesystem>
#include "basicCubeMesh.h"
#include "camera.h"

//using namespace std;

Camera camera = Camera();

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

float lastX = 400, lastY = 300;
bool firstMouse = true;


int main()
{
#pragma region GLFW_INIT_&_SETUP
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


    GLFWwindow* window = glfwCreateWindow(800, 600, "MyGameEngine", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);//****** changes

    glViewport(0, 0, 800, 600);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

#pragma endregion

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //instantiate shader 
    Shader myShader("resources/shaders/basic.shader.vert", "resources/shaders/basic.shader.frag");

    //load texture
#pragma region TEXTURE

    unsigned int texture1;
    std::filesystem::path imagePath = "resources/textures/ChessboardTexture.png";

    //texture 1

    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char* data = stbi_load(imagePath.generic_string().c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
#pragma endregion


    //after setup 
    //define model properties


    //convert vertices into basic vertx class
    //std::vector<basicVertex> vertices = {
    //    {glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f,0.0f)},
    //    {glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f,0.0f)},
    //    {glm::vec3(0.0f,  0.5f, 0.0), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.5f,1.0f)}
    //};

    //basicMesh myTriangle(vertices);
    std::vector<basicCubeVertex> vertices = {
        {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3(0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f)},

        {glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3(0.5f, -0.5f,  0.5f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3(0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f)},

        {glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f)},

        {glm::vec3(0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3(0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3(0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f)},

        {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(0.5f, -0.5f,  0.5f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3(0.5f, -0.5f,  0.5f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f)},

        {glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3(0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f)},
    };

    std::vector<basicCubeVertex> verticesSequel = {
       {glm::vec3(-0.5f, -0.5f, -1.5f), glm::vec2(0.0f, 0.0f)},
       {glm::vec3(0.5f, -0.5f, -1.5f), glm::vec2(1.0f, 0.0f)},
       {glm::vec3(0.5f,  0.5f, -1.5f), glm::vec2(1.0f, 1.0f)},
       {glm::vec3(0.5f,  0.5f, -1.5f), glm::vec2(1.0f, 1.0f)},
       {glm::vec3(-0.5f,  0.5f, -1.5f), glm::vec2(0.0f, 1.0f)},
       {glm::vec3(-0.5f, -0.5f, -1.5f), glm::vec2(0.0f, 0.0f)},

       {glm::vec3(-0.5f, -0.5f,  -0.5f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3(0.5f, -0.5f,  -0.5f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3(0.5f,  0.5f,  -0.5f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(0.5f,  0.5f,  -0.5f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(-0.5f,  0.5f,  -0.5f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(-0.5f, -0.5f,  -0.5f), glm::vec2(0.0f, 0.0f)},

       {glm::vec3(-0.5f,  0.5f,  -1.5f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(-0.5f, -0.5f,  -1.5f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3(-0.5f,  0.5f,  -1.5f), glm::vec2(1.0f, 0.0f)},

       {glm::vec3(0.5f,  0.5f,  -1.5f), glm::vec2(1.0f, 0.0f)},
       {glm::vec3(0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f)},
       {glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f)},
       {glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f)},
       {glm::vec3(0.5f, -0.5f,  -1.5f), glm::vec2(0.0f, 0.0f)},
       {glm::vec3(0.5f,  0.5f,  -1.5f), glm::vec2(1.0f, 0.0f)},

       {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(0.5f, -0.5f,  0.5f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3(0.5f, -0.5f,  0.5f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f)},

       {glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f)},
       {glm::vec3(0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f)},
       {glm::vec3(0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f)},
       {glm::vec3(0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f)},
       {glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(0.0f, 0.0f)},
       {glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f)},
    };
    basicCubeMesh myCube(vertices);
    basicCubeMesh myCubeSequel(verticesSequel);



    while (!glfwWindowShouldClose(window))
    {
        //inputs
        processInput(window);

        //processing
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, texture1);

        // cube
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        unsigned int viewLoc = glGetUniformLocation(myShader.ID, "view");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

        myShader.setMat4("projection", projection);
        myShader.setMat4("model", model);
        myCube.Draw(myShader);
        myCubeSequel.Draw(myShader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;

    //return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
        camera.toggleFreeLook();  // Toggle free look when the TAB key is pressed

    if (camera.isFreeLook()) {
        camera.processInput(window);  // Process free look camera input
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    camera.processKeyboardInput(key, action);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
    lastX = xpos;
    lastY = ypos;

    if (camera.isFreeLook())
        camera.processMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (camera.isFreeLook())
        camera.processMouseScroll(yoffset);
}
