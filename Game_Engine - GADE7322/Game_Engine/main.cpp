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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    camera.processKeyboardInput(key, action);
}


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

#pragma endregion

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //instantiate shader 
    Shader myShader("resources/shaders/basic.shader.vert", "resources/shaders/basic.shader.frag");
    Shader myShaderSequel("resources/shaders/basic.shader.vert", "resources/shaders/basic.shader.frag");

    //load texture
#pragma region TEXTURE

    unsigned int texture1;
    std::filesystem::path imagePath = "resources/textures/garfield.png";

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
        //1
        {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3(0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f)},

        //2
        {glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3(0.5f, -0.5f,  0.5f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3(0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f)},

        //3
        {glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f)},

        //4
        {glm::vec3(0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3(0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3(0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f)},

        //5
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

    glm::vec3 cubePositions[] =
    {
        //1
        glm::vec3(0.0f, 13.0f, 0.0f),//1
        glm::vec3(1.0f, 13.0f, 0.0f),//2
        glm::vec3(2.0f, 13.1f, 0.0f),//3
        glm::vec3(3.0f, 13.2f, 0.0f),//4
        glm::vec3(4.0f, 13.3f, 0.0f),//5
        glm::vec3(-1.0f, 13.1f, 0.0f),//6
        glm::vec3(-2.0f, 13.2f, 0.0f),//7
        glm::vec3(-3.0f, 13.3f, 0.0f),//8
        //2
        glm::vec3(1.0f, 13.0f, 1.0f),//1
        glm::vec3(0.0f, 13.0f, 1.0f),//2
        glm::vec3(3.0f, 13.1f, 1.0f),//3
        glm::vec3(2.0f, 13.2f, 1.0f),//4
        glm::vec3(-1.0f, 13.3f, 1.0f),//5
        glm::vec3(4.0f, 13.1f, 1.0f),//6
        glm::vec3(-3.0f, 13.2f, 1.0f),//7
        glm::vec3(-2.0f, 13.3f, 1.0f),//8
        //3
        glm::vec3(0.0f, 13.0f, 2.0f),//1
        glm::vec3(1.0f, 13.0f, 2.0f),//2
        glm::vec3(2.0f, 13.1f, 2.0f),//3
        glm::vec3(3.0f, 13.2f, 2.0f),//4
        glm::vec3(4.0f, 13.3f, 2.0f),//5
        glm::vec3(-1.0f, 13.1f, 2.0f),//6
        glm::vec3(-2.0f, 13.2f, 2.0f),//7
        glm::vec3(-3.0f, 13.3f, 2.0f),//8
        //4
        glm::vec3(1.0f, 13.0f, 3.0f),//1
        glm::vec3(0.0f, 13.0f, 3.0f),//2
        glm::vec3(3.0f, 13.1f, 3.0f),//3
        glm::vec3(2.0f, 13.2f, 3.0f),//4
        glm::vec3(-1.0f, 13.3f, 3.0f),//5
        glm::vec3(4.0f, 13.1f, 3.0f),//6
        glm::vec3(-3.0f, 13.2f, 3.0f),//7
        glm::vec3(-2.0f, 13.3f, 3.0f),//8
        //5
        glm::vec3(0.0f, 13.0f, -1.0f),//1
        glm::vec3(1.0f, 13.0f, -1.0f),//2
        glm::vec3(2.0f, 13.1f, -1.0f),//3
        glm::vec3(3.0f, 13.2f, -1.0f),//4
        glm::vec3(4.0f, 13.3f, -1.0f),//5
        glm::vec3(-1.0f, 13.1f, -1.0f),//6
        glm::vec3(-2.0f, 13.2f, -1.0f),//7
        glm::vec3(-3.0f, 13.3f, -1.0f),//8
        //6
        glm::vec3(1.0f, 13.0f, -2.0f),//1
        glm::vec3(0.0f, 13.0f, -2.0f),//2
        glm::vec3(3.0f, 13.1f, -2.0f),//3
        glm::vec3(2.0f, 13.2f, -2.0f),//4
        glm::vec3(-1.0f, 13.3f, -2.0f),//5
        glm::vec3(4.0f, 13.1f, -2.0f),//6
        glm::vec3(-3.0f, 13.2f, -2.0f),//7
        glm::vec3(-2.0f, 13.3f, -2.0f),//8
        //7
        glm::vec3(0.0f, 13.0f, -3.0f),//1
        glm::vec3(1.0f, 13.0f, -3.0f),//2
        glm::vec3(2.0f, 13.1f, -3.0f),//3
        glm::vec3(3.0f, 13.2f, -3.0f),//4
        glm::vec3(4.0f, 13.3f, -3.0f),//5
        glm::vec3(-1.0f, 13.1f, -3.0f),//6
        glm::vec3(-2.0f, 13.2f, -3.0f),//7
        glm::vec3(-3.0f, 13.3f, -3.0f),//8
    };

    glm::vec3 cubePositions2[] =
    {
        //1
        glm::vec3(4.5f, 12.5f, 4.5f),//1
        glm::vec3(3.5f, 12.5f, 4.5f),//2
        glm::vec3(2.5f, 12.5f, 4.5f),//3
        glm::vec3(1.5f, 12.5f, 4.5f),//4
        glm::vec3(0.5f, 12.5f, 4.5f),//5
        glm::vec3(-0.5f, 12.5f, 4.5f),//6
        glm::vec3(-1.5f, 12.5f, 4.5f),//7
        glm::vec3(-2.5f, 12.5f, 4.5f),//8
        glm::vec3(-3.5f, 12.5f, 4.5f),//9
        //2
        glm::vec3(4.5f, 12.5f, 3.5f),//1
        glm::vec3(4.5f, 12.5f, 2.5f),//2
        glm::vec3(4.5f, 12.5f, 1.5f),//3
        glm::vec3(4.5f, 12.5f, 0.5f),//4
        glm::vec3(4.5f, 12.5f, -0.5f),//5
        glm::vec3(4.5f, 12.5f, -1.5f),//6
        glm::vec3(4.5f, 12.5f, -2.5f),//7
        glm::vec3(4.5f, 12.5f, -3.5f),//8
        //3
        glm::vec3(4.5f, 12.5f, -3.5f),//1
        glm::vec3(3.5f, 12.5f, -3.5f),//2
        glm::vec3(2.5f, 12.5f, -3.5f),//3
        glm::vec3(1.5f, 12.5f, -3.5f),//4
        glm::vec3(0.5f, 12.5f, -3.5f),//5
        glm::vec3(-0.5f, 12.5f, -3.5f),//6
        glm::vec3(-1.5f, 12.5f, -3.5f),//7
        glm::vec3(-2.5f, 12.5f, -3.5f),//8
        glm::vec3(-3.5f, 12.5f, -3.5f),//9
        //4
        glm::vec3(-3.5f, 12.5f, 3.5f),//1
        glm::vec3(-3.5f, 12.5f, 2.5f),//2
        glm::vec3(-3.5f, 12.5f, 1.5f),//3
        glm::vec3(-3.5f, 12.5f, 0.5f),//4
        glm::vec3(-3.5f, 12.5f, -0.5f),//5
        glm::vec3(-3.5f, 12.5f, -1.5f),//6
        glm::vec3(-3.5f, 12.5f, -2.5f),//7
        glm::vec3(-3.5f, 12.5f, -3.5f),//8        
    };

    while (!glfwWindowShouldClose(window))
    {
        //inputs
        processInput(window);


        //processing
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //RGB values to change colour
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//******* changes

        glBindTexture(GL_TEXTURE_2D, texture1);

        //=============================================
        //cube 
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 modelSequel = glm::mat4(1.0f);
        //model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        //modelSequel = glm::rotate(modelSequel, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        //modelSequel = glm::translate(modelSequel, glm::vec3(0.0f, 0.0f, 1.0f));

        glm::mat4 view = glm::mat4(1.0f);
        // note that we're translating the scene in the reverse direction of where we want to move
        view = camera.getViewMatrix();

        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        unsigned int viewLoc = glGetUniformLocation(myShader.ID, "view");
        unsigned int viewLocSequel = glGetUniformLocation(myShaderSequel.ID, "view");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(viewLocSequel, 1, GL_FALSE, &view[0][0]);

        myShader.setMat4("projection", projection);
        myShader.setMat4("model", model);
        myShaderSequel.setMat4("projection", projection);
        myShaderSequel.setMat4("model", modelSequel);
        myCube.Draw(myShader);



        //math

        //frame buffers

        glfwSwapBuffers(window);
        glfwPollEvents();

        glfwSetKeyCallback(window, key_callback);
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


    //all input managing things
}
