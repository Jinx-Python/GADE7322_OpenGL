#ifndef BASICMESH_H
#define BASICMESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"

#include <string>
#include <vector>

using namespace std;

//template class for structure, this holds vertex information for this specific mesh
//name this according to your mesh eg if you called your mesh triangle mesh name struct triangle vertex
struct basicVertex { // this mesh is called basicMesh
	
	//position attribute
	glm::vec3 Position;

	//colour attribute
	glm::vec3 Colours;

	//texture co-ordinates
	glm::vec2 texture;
};

class basicMesh {

public:
	vector<basicVertex> vertices;

	unsigned int VAO;

	//constructor
	basicMesh(vector<basicVertex> vertices)
	{
		this->vertices = vertices;

		setupMesh();
	}

	//draw function
	void Draw(Shader& shader) //mesh needs to know what type of shader is being used
	{
		shader.use();
        //shader.setVec3("colourIn", glm::vec3(1.0f, 0.0f, 0.0f));// sets color to red
        //shader.setVec3("colourIn", glm::vec3(tan(glfwGetTime()), tan(glfwGetTime()), tan(glfwGetTime())) ); //randomise between black and white over time
        shader.setVec3("colourIn", glm::vec3(sin(glfwGetTime()), cos(glfwGetTime()), tan(glfwGetTime()))); //randomise colours over time
        //glUseProgram(shaderProgram); // this binds the shader
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
	}


private:

	//setting up your mesh
	unsigned int VBO, EBO;
	void setupMesh()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);


		//generate buffer data
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * sizeof(basicVertex), &vertices[0], GL_DYNAMIC_DRAW); // gl draw can be static or dynaic,static is great for memory but you cant change anything. dynaic is not great on memory but you change change things

		///Attribute pointers
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(basicVertex), (void*)offsetof(basicVertex, Position));
		glEnableVertexAttribArray(0);

		//colour attributes
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(basicVertex), (void*)offsetof(basicVertex, texture));
		glEnableVertexAttribArray(1);
	}
};


#endif // !BASICMESH_H

