#pragma once
#ifndef SHADER_H
#define SHADER_H
//#endif // ! SHADER_H


#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include <iostream>
#include<fstream>
#include<sstream> 


class Shader {
public:
	//program ID
	unsigned int ID;

	//constructor
	Shader(const char* vertexPath, const char* fragmentPath);


	void use();


	//utility functions

	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, bool value) const;
	void setFloat(const std::string& name, bool value) const;

	void setVec3(const std::string& name, const glm::vec3& value) const;
	void setVec4(const std::string& name, const float x, float y, float z, float w) const;
	void setMat4(const std::string& name, const glm::mat4& mat) const;


private:

	void checkCompilerErrors(unsigned int shader, std::string type);
};
#endif
