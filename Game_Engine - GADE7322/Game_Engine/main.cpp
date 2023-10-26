#define STB_IMAGE_IMPLEMENTATION
//#define M_PI 3.14159265358979323846
#define _USE_MATH_DEFINES
#include <cmath>
#include "stb_image.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include "shader.h"
#include <vector>
#include "basicMesh.h"
#include <filesystem>
#include "basicCubeMesh.h"
#include "camera.h"
#include "ObjectContainer.h"
#include "AnimationController.cpp"

//using namespace std;

//glEnable(GL_COLOR_MATERIAL);
//glColorMaterial(GL_FRONT, GL_DIFFUSE);
//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

GLuint sphereVAO, cylinderVAO, cubeVAO, coneVAO;
std::vector<glm::vec3> sphereVertices, cylinderVertices, cubeVertices, coneVetrices;

GLuint shaderProgram;
GLuint vao, vbo;
glm::mat4 model = glm::mat4(1.0f);
glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

void createPawn();
//void renderPawn();
void createRook();
//void renderRook();
void createKnight();
//void renderKnight();
void createBishop();
//void renderBishop();
void createKing();
//void renderKing();
void createQueen();
//void renderQueen();


Camera camera = Camera();

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    camera.processKeyboardInput(key, action);
}

//chessboard vertices
float vertices[] = {
    // positions         // colors
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
};

// Vertex shader source code
const char* vertexShaderSource = R"glsl(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 aTexCoord;

    void main()
    {
        gl_Position = vec4(aPos, 1.0);
    }
)glsl";

// Fragment shader source code
const char* fragmentShaderSource = R"glsl(
    #version 330 core
    out vec4 FragColor;

    void main()
    {
        FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    }
)glsl";

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



    // Create and compile the vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

#pragma endregion

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //instantiate shader 
    Shader myShader("resources/shaders/basic.shader.vert", "resources/shaders/basic.shader.frag");    

    //load texture
#pragma region TEXTURE

    unsigned int texture1;
<<<<<<< Updated upstream
    //std::filesystem::path imagePath = "resources/textures/garfield.png";
=======
    std::filesystem::path imagePath = "resources/textures/ChessboardTexture.png";
>>>>>>> Stashed changes

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
    //unsigned char* data = stbi_load(imagePath.generic_string().c_str(), &width, &height, &nrChannels, 0);
    
#pragma endregion


    //after setup 
    //define model properties   

    //basicMesh myTriangle(vertices);  

    createPawn();
    createBishop();
    createKnight();
    createRook();
    createQueen();
    createKing();

<<<<<<< Updated upstream
    AnimationController anim;
=======
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
        glm::vec3(0.0f, 13.0f, 0.0f),
        glm::vec3(1.0f, 13.0f, 0.0f),
        glm::vec3(2.0f, 13.1f, 0.0f),
        glm::vec3(3.0f, 13.2f, 0.0f),
        glm::vec3(4.0f, 13.3f, 0.0f),
        glm::vec3(-1.0f, 13.1f, 0.0f),
        glm::vec3(-2.0f, 13.2f, 0.0f),
        glm::vec3(-3.0f, 13.3f, 0.0f),

        //2
        glm::vec3(1.0f, 13.0f, 1.0f),
        glm::vec3(0.0f, 13.0f, 1.0f),
        glm::vec3(3.0f, 13.1f, 1.0f),
        glm::vec3(2.0f, 13.2f, 1.0f),
        glm::vec3(-1.0f, 13.3f, 1.0f),
        glm::vec3(4.0f, 13.1f, 1.0f),
        glm::vec3(-3.0f, 13.2f, 1.0f),
        glm::vec3(-2.0f, 13.3f, 1.0f),

        //3
        glm::vec3(0.0f, 13.0f, 2.0f),
        glm::vec3(1.0f, 13.0f, 2.0f),
        glm::vec3(2.0f, 13.1f, 2.0f),
        glm::vec3(3.0f, 13.2f, 2.0f),
        glm::vec3(4.0f, 13.3f, 2.0f),
        glm::vec3(-1.0f, 13.1f, 2.0f),
        glm::vec3(-2.0f, 13.2f, 2.0f),
        glm::vec3(-3.0f, 13.3f, 2.0f),

        //4
        glm::vec3(1.0f, 13.0f, 3.0f),
        glm::vec3(0.0f, 13.0f, 3.0f),
        glm::vec3(3.0f, 13.1f, 3.0f),
        glm::vec3(2.0f, 13.2f, 3.0f),
        glm::vec3(-1.0f, 13.3f, 3.0f),
        glm::vec3(4.0f, 13.1f, 3.0f),
        glm::vec3(-3.0f, 13.2f, 3.0f),
        glm::vec3(-2.0f, 13.3f, 3.0f),

        //5
        glm::vec3(0.0f, 13.0f, -1.0f),
        glm::vec3(1.0f, 13.0f, -1.0f),
        glm::vec3(2.0f, 13.1f, -1.0f),
        glm::vec3(3.0f, 13.2f, -1.0f),
        glm::vec3(4.0f, 13.3f, -1.0f),
        glm::vec3(-1.0f, 13.1f, -1.0f),
        glm::vec3(-2.0f, 13.2f, -1.0f),
        glm::vec3(-3.0f, 13.3f, -1.0f),

        //6
        glm::vec3(1.0f, 13.0f, -2.0f),
        glm::vec3(0.0f, 13.0f, -2.0f),
        glm::vec3(3.0f, 13.1f, -2.0f),
        glm::vec3(2.0f, 13.2f, -2.0f),
        glm::vec3(-1.0f, 13.3f, -2.0f),
        glm::vec3(4.0f, 13.1f, -2.0f),
        glm::vec3(-3.0f, 13.2f, -2.0f),
        glm::vec3(-2.0f, 13.3f, -2.0f),

        //7
        glm::vec3(0.0f, 13.0f, -3.0f),
        glm::vec3(1.0f, 13.0f, -3.0f),
        glm::vec3(2.0f, 13.1f, -3.0f),
        glm::vec3(3.0f, 13.2f, -3.0f),
        glm::vec3(4.0f, 13.3f, -3.0f),
        glm::vec3(-1.0f, 13.1f, -3.0f),
        glm::vec3(-2.0f, 13.2f, -3.0f),
        glm::vec3(-3.0f, 13.3f, -3.0f),
    };

    glm::vec3 cubePositions2[] =
    {
        //1
        glm::vec3(4.5f, 12.5f, 4.5f),
        glm::vec3(3.5f, 12.5f, 4.5f),
        glm::vec3(2.5f, 12.5f, 4.5f),
        glm::vec3(1.5f, 12.5f, 4.5f),
        glm::vec3(0.5f, 12.5f, 4.5f),
        glm::vec3(-0.5f, 12.5f, 4.5f),
        glm::vec3(-1.5f, 12.5f, 4.5f),
        glm::vec3(-2.5f, 12.5f, 4.5f),
        glm::vec3(-3.5f, 12.5f, 4.5f),

        //2
        glm::vec3(4.5f, 12.5f, 3.5f),
        glm::vec3(4.5f, 12.5f, 2.5f),
        glm::vec3(4.5f, 12.5f, 1.5f),
        glm::vec3(4.5f, 12.5f, 0.5f),
        glm::vec3(4.5f, 12.5f, -0.5f),
        glm::vec3(4.5f, 12.5f, -1.5f),
        glm::vec3(4.5f, 12.5f, -2.5f),
        glm::vec3(4.5f, 12.5f, -3.5f),

        //3
        glm::vec3(4.5f, 12.5f, -3.5f),
        glm::vec3(3.5f, 12.5f, -3.5f),
        glm::vec3(2.5f, 12.5f, -3.5f),
        glm::vec3(1.5f, 12.5f, -3.5f),
        glm::vec3(0.5f, 12.5f, -3.5f),
        glm::vec3(-0.5f, 12.5f, -3.5f),
        glm::vec3(-1.5f, 12.5f, -3.5f),
        glm::vec3(-2.5f, 12.5f, -3.5f),
        glm::vec3(-3.5f, 12.5f, -3.5f),

        //4
        glm::vec3(-3.5f, 12.5f, 3.5f),
        glm::vec3(-3.5f, 12.5f, 2.5f),
        glm::vec3(-3.5f, 12.5f, 1.5f),
        glm::vec3(-3.5f, 12.5f, 0.5f),
        glm::vec3(-3.5f, 12.5f, -0.5f),
        glm::vec3(-3.5f, 12.5f, -1.5f),
        glm::vec3(-3.5f, 12.5f, -2.5f),
        glm::vec3(-3.5f, 12.5f, -3.5f),       
    };
    
    //Pawn
    //Head (sphere)
    //GLfloat sphereVertices[] = { /* vertices for the sphere */ };
    //GLuint sphereVAO, sphereVBO;
    //glGenVertexArrays(1, &sphereVAO);
    //glGenBuffers(1, &sphereVBO);
    //glBindVertexArray(sphereVAO);
    //glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(sphereVertices), sphereVertices, GL_STATIC_DRAW);

    //Body (cylinder)
    //GLfloat cylinderVertices[] = { /* vertices for the cylinder */ };
    //GLuint cylinderVAO, cylinderVBO;
    //glGenVertexArrays(1, &cylinderVAO);
    //glGenBuffers(1, &cylinderVBO);
    //glBindVertexArray(cylinderVAO);
    //glBindBuffer(GL_ARRAY_BUFFER, cylinderVBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(cylinderVertices), cylinderVertices, GL_STATIC_DRAW); 
    
    
>>>>>>> Stashed changes

    while (!glfwWindowShouldClose(window))
    {       

        //inputs
        processInput(window);

        //when you normally create objects
        ObjectContainer myObject;
        /*basicCubeMesh myCube(vertices, indices);
        basicCubeMesh myTriangle(pyramidVerts, pyramidInd);
        myObject.addMesh(myCube,
            glm::vec3(0, 0, 0),
            glm::vec3(0, 0, 0),
            glm::vec3(1, 1, 1));
        myObject.addMesh(myTriangle,
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(0, 0, 0),
            glm::vec3(1.5f, 1.5f, 1.5f));

        //NOTE you still need to do animation on the model in main like normal
        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        anim.applyComplicatedAnimation(model);*/

        //but then you can just draw the container -- INSTEAD of the objects
        myObject.Draw(model, myShader);


        //processing
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //RGB values to change colour
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//******* changes
        

        glBindTexture(GL_TEXTURE_2D, texture1);

        //=============================================
        //cube 
        glm::mat4 model = glm::mat4(1.0f);
        //model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        //model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
<<<<<<< Updated upstream
        //model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
=======
        //modelSequel = glm::rotate(modelSequel, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        //modelSequel = glm::translate(modelSequel, glm::vec3(0.0f, 0.0f, 1.0f));
>>>>>>> Stashed changes

        glm::mat4 view = glm::mat4(1.0f);
        // note that we're translating the scene in the reverse direction of where we want to move
        view = camera.getViewMatrix();

        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        unsigned int viewLoc = glGetUniformLocation(myShader.ID, "view");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

        myShader.setMat4("projection", projection);
        myShader.setMat4("model", model);

        /*renderPawn();
        renderRook();
        renderKnight();
        renderBishop();
        renderQueen();
        renderKing();*/


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

<<<<<<< Updated upstream
void drawCube(float size) {
    float halfSize = size / 2.0f;

    // Define the vertices of the cube
    GLfloat vertices[] = {
        -halfSize, -halfSize, halfSize,
        halfSize, -halfSize, halfSize,
        halfSize, halfSize, halfSize,
        -halfSize, halfSize, halfSize,
        -halfSize, -halfSize, -halfSize,
        halfSize, -halfSize, -halfSize,
        halfSize, halfSize, -halfSize,
        -halfSize, halfSize, -halfSize
    };

    // Define the indices of the cube
    GLuint indices[] = {
        0, 1, 2, 2, 3, 0, // Front face
        1, 5, 6, 6, 2, 1, // Right face
        7, 6, 5, 5, 4, 7, // Back face
        4, 0, 3, 3, 7, 4, // Left face
        4, 5, 1, 1, 0, 4, // Bottom face
        3, 2, 6, 6, 7, 3  // Top face
    };

    // Generate and bind the VAO
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Generate and bind the VBO
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Generate and bind the EBO
    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind the VAO
    glBindVertexArray(0);
}

void drawSphere(float radius, int stacks, int slices) {
    std::vector<glm::vec3> vertices;
    std::vector<GLuint> indices;
    for (int i = 0; i <= stacks; ++i) {
        float V = i / (float)stacks;
        float phi = V * glm::pi<float>();
        for (int j = 0; j <= slices; ++j) {
            float U = j / (float)slices;
            float theta = U * (glm::pi<float>() * 2);
            float x = cosf(theta) * sinf(phi);
            float y = cosf(phi);
            float z = sinf(theta) * sinf(phi);
            vertices.push_back(glm::vec3(x, y, z) * radius);
        }
    }
    for (int i = 0; i < slices * stacks + slices; ++i) {
        indices.push_back(i);
        indices.push_back(i + slices + 1);
        indices.push_back(i + slices);
        indices.push_back(i + slices + 1);
        indices.push_back(i);
        indices.push_back(i + 1);
    }

    // Generate and bind the VAO
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Generate and bind the VBO
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    // Generate and bind the EBO
    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind the VAO
    glBindVertexArray(0);
}

void drawCylinder(float radius, float height, int stacks, int slices) {
    std::vector<glm::vec3> vertices;
    std::vector<GLuint> indices;
    for (int i = 0; i < slices; ++i) {
        GLfloat u = i / (GLfloat)slices;
        GLfloat x = radius * cos(2 * M_PI * u);
        GLfloat z = radius * sin(2 * M_PI * u);
        vertices.push_back(glm::vec3(x, 0.0f, z));
        vertices.push_back(glm::vec3(x, height, z));
    }
    for (int i = 0; i < slices; ++i) {
        GLuint i1 = i;
        GLuint i2 = (i + 1) % slices;
        indices.push_back(i1);
        indices.push_back(i2);
    }
    for (int i = 0; i < slices; ++i) {
        GLuint i1 = i;
        GLuint i2 = (i + 1) % slices;
        indices.push_back(i1 + slices);
        indices.push_back(i2 + slices);
    }

    // Generate and bind the VAO
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Generate and bind the VBO
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    // Generate and bind the EBO
    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind the VAO
    glBindVertexArray(0);
}

void drawCone(float radius, float height, int slices) {
    std::vector<glm::vec3> vertices;
    std::vector<GLuint> indices;
    for (int i = 0; i <= slices; ++i) {
        float U = i / (float)slices;
        float theta = U * (glm::pi<float>() * 2);
        float x = cosf(theta) * radius;
        float z = sinf(theta) * radius;
        vertices.push_back(glm::vec3(x, 0.0f, z));
        vertices.push_back(glm::vec3(x, height, z));
    }
    for (int i = 0; i < slices; ++i) {
        GLuint i1 = i;
        GLuint i2 = (i + 1) % slices;
        indices.push_back(i1);
        indices.push_back(i2);
    }
    for (int i = 0; i < slices; ++i) {
        GLuint i1 = i;
        GLuint i2 = (i + 1) % slices;
        indices.push_back(i1 + slices);
        indices.push_back(i2 + slices);
    }

    // Generate and bind the VAO
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Generate and bind the VBO
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    // Generate and bind the EBO
    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind the VAO
    glBindVertexArray(0);
}

void createPawn() {
    // Draw the head (sphere)
    drawSphere(1.0f, 10, 10);
    sphereVAO = vao;
    //sphereVertices = vertices;

    // Draw the body (cylinder)
    drawCylinder(1.0f, 2.0f, 10, 10);
    cylinderVAO = vao;
    //cylinderVertices = vertices;
}

/*void renderPawn() {
    glUseProgram(shaderProgram);

    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLuint projLoc = glGetUniformLocation(shaderProgram, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Draw the head (sphere)
    glBindVertexArray(sphereVAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, sphereVertices.size());
    glBindVertexArray(0);

    // Draw the body (cylinder)
    glBindVertexArray(cylinderVAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, cylinderVertices.size());
    glBindVertexArray(0);
}*/

void createRook() {
    // Draw the body (cylinder)
    drawCylinder(1.0f, 2.0f, 10, 10);

    // Draw the head (cube)
    drawCube(1.0f);
}

/*void renderRook() {
    glUseProgram(shaderProgram);

    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLuint projLoc = glGetUniformLocation(shaderProgram, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 18);
    glBindVertexArray(0);
}*/

void createKnight() {
    // Draw the body (cylinder)
    drawCylinder(1.0f, 2.0f, 10, 10);

    // Draw the head (cube)
    drawCube(1.0f);
}

/*void renderKnight() {
    glUseProgram(shaderProgram);

    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLuint projLoc = glGetUniformLocation(shaderProgram, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 7);
    glBindVertexArray(0);
}*/

void createBishop() {
    // Draw the body (cylinder)
    drawCylinder(1.0f, 2.0f, 10, 10);

    // Draw the head (sphere)
    drawSphere(1.0f, 10, 10);
}

/*void renderBishop() {
    glUseProgram(shaderProgram);

    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLuint projLoc = glGetUniformLocation(shaderProgram, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 7);
    glBindVertexArray(0);
}*/

void createQueen() {
    // Draw the body (cylinder)
    drawCylinder(1.0f, 2.0f, 10, 10);

    // Draw the head (sphere)
    drawSphere(1.0f, 10, 10);
}

/*void renderQueen() {
    glUseProgram(shaderProgram);

    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");

    //Position the queen
    model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 9);
    glBindVertexArray(0);
}*/

void createKing() {
    // Draw the body (cylinder)
    drawCylinder(1.0f, 2.0f, 10, 10);

    // Draw the head (cube)
    drawCube(1.0f);
}

/*void renderKing() {
    glUseProgram(shaderProgram);

    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLuint projLoc = glGetUniformLocation(shaderProgram, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 7);
    glBindVertexArray(0);
}*/

=======
>>>>>>> Stashed changes
