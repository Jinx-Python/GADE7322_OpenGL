#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define FREEGLUT_STATIC
#define _LIB
#define FREEGLUT_LIB_PRAGMAS 0
#include <GL/glut.h>
#include <iostream>
#include "shader.h"
#include <vector>
#include "basicMesh.h"
#include <filesystem>
#include "basicCubeMesh.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


int main()
{
#pragma region GLFW_INIT_&_SETUP
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);    

    int mapWidth, mapHeight, nChannels;
    unsigned char* mapData = stbi_load("resources/textures/HightMap.png",
                                        &mapWidth, &mapHeight, &nChannels, 0);

    // vertex generation
    std::vector<float> vertices;
    float yScale = 64.0f / 256.0f, yShift = 16.0f;  // apply a scale+shift to the height data
    int rez = 1;
    for (unsigned int i = 0; i < mapHeight; i++)
    {
        for (unsigned int j = 0; j < mapWidth; j++)
        {
            // retrieve texel for (i,j) tex coord
            unsigned char* texel = mapData + (j + mapWidth * i) * nChannels;
            // raw height at coordinate
            unsigned char y = texel[0];

            // vertex
            vertices.push_back(-mapHeight / 2.0f + mapHeight * i / (float)mapHeight );        // v.x
            vertices.push_back((int)y * yScale - yShift); // v.y
            vertices.push_back(-mapWidth / 2.0f + mapWidth * j / (float)mapWidth );        // v.z
        }
    }

    stbi_image_free(mapData);

    // index generation
    std::vector<unsigned> indices;
    for (unsigned i = 0; i < mapHeight - 1; i+= rez)       // for each row a.k.a. each strip
    {
        for (unsigned j = 0; j < mapWidth; j+= rez)      // for each column
        {
            for (unsigned int k = 0; k < 2; k++)      // for each side of the strip
            {
                indices.push_back(j + mapWidth * (i + k * rez));
            }
        }
    }
    std::cout << "Loaded " << indices.size() << " indices" << std::endl;

    const unsigned int NUM_STRIPS = mapHeight - 1;
    const unsigned int NUM_VERTS_PER_STRIP = mapWidth * 2;
    std::cout << "Created lattice of " << NUM_STRIPS << " strips with " << NUM_VERTS_PER_STRIP << " triangles each" << std::endl;
    std::cout << "Created " << NUM_STRIPS * NUM_VERTS_PER_STRIP << " triangles total" << std::endl;    

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
    Shader heightMapShader("8.3.cpuheight.vs", "8.3.cpuheight.fs");

    // register VAO
    unsigned int terrainVAO, terrainVBO, terrainEBO;
    glGenVertexArrays(1, &terrainVAO);
    glBindVertexArray(terrainVAO);


    glGenBuffers(1, &terrainVBO);
    glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
    glBufferData(GL_ARRAY_BUFFER,
        vertices.size() * sizeof(float),       // size of vertices buffer
        &vertices[0],                          // pointer to first element
        GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &terrainEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(unsigned int), // size of indices buffer
        &indices[0],                           // pointer to first element
        GL_STATIC_DRAW);

    // draw mesh
    glBindVertexArray(terrainVAO);
    // render the mesh triangle strip by triangle strip - each row at a time
    for (unsigned int strip = 0; strip < NUM_STRIPS; ++strip)
    {
        glDrawElements(GL_TRIANGLE_STRIP,   // primitive type
            NUM_VERTS_PER_STRIP, // number of indices to render
            GL_UNSIGNED_INT,     // index data type
            (void*)(sizeof(unsigned int)
                * NUM_VERTS_PER_STRIP
                * strip)); // offset to starting index
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //instantiate shader 
    Shader myShader("resources/shaders/basic.shader.vert", "resources/shaders/basic.shader.frag");

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

    const int numStrips = (height - 1) / rez;
    const int numTrisPerStrip = (width / rez) * 2 - 2;

    while (!glfwWindowShouldClose(window))
    {
        //inputs
        processInput(window);

        //processing
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //RGB values to change colour
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//******* changes

        heightMapShader.use();         

        glBindTexture(GL_TEXTURE_2D, texture1);

        //define vector  and initialize it to an identity matrix
        //glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
        //glm::mat4 trans = glm::mat4(1.0);

        //create transformation matrix 
        //trans = glm::translate(trans, glm::vec3(-0.5f, -0.5f, 0.0f));
        //trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

        //vec = trans * vec;

        //std::cout << vec.x << vec.y << vec.z << std::endl;

        //pass the  transformation matrix to the shader
        //unsigned int transformLoc = glGetUniformLocation(myShader.ID, "transform");

        //get matrix location  and set matrix
        //glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

        //myShader.setMat4("transform", trans);
       // myTriangle.Draw(myShader);


        //=============================================
        //cube 
        glm::mat4 model = glm::mat4(1.0f);
        //model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        //model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

        glm::mat4 view = glm::mat4(1.0f);
        // note that we're translating the scene in the reverse direction of where we want to move
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        unsigned int viewLoc = glGetUniformLocation(heightMapShader.ID, "view");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

        myShader.setMat4("projection", projection);
        myShader.setMat4("model", model);

        // world transformation
        heightMapShader.setMat4("model", model);

        heightMapShader.setMat4("projection", projection);
        heightMapShader.setMat4("view", view);

        //draw terrain
        glBindVertexArray(terrainVAO);
        for (unsigned strip = 0; strip < numStrips; strip++)
        {
            glDrawElements(GL_TRIANGLE_STRIP,   // primitive type
                numTrisPerStrip + 2,   // number of indices to render
                GL_UNSIGNED_INT,     // index data type
                (void*)(sizeof(unsigned) * (numTrisPerStrip + 2) * strip)); // offset to starting index
        }

        glBindVertexArray(0);

        //math

        //frame buffers

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

    //all input managing things
}