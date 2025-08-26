#include <iostream>
#include <Shader.hpp>
#include "fstream"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
using std::string;

Shader::Shader(const string& vPath, const string& fPath){
	std::cout << "created an instance of shader manager\n";
	
	string vSrc = parseShader(vPath);	
	string fSrc = parseShader(fPath);	
	const char* cVertexSrc = vSrc.c_str(); 	
	const char* cFragmentSrc = fSrc.c_str(); 	

	unsigned int vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &cVertexSrc, NULL);
	glCompileShader(vShader);
	
	int success;
	char infoLog[512];
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
	if(!success){
		glGetShaderInfoLog(vShader, 512, NULL, infoLog);
		std::cout << "Failed to compile vertex shader\n" << infoLog << '\n';
	}

	unsigned int fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &cFragmentSrc, NULL);
	glCompileShader(fShader);


	glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
	if(!success){
		glGetShaderInfoLog(fShader, 512, NULL, infoLog);
		std::cout << "Failed to compile fragment shader\n" << infoLog << '\n';
	}

	ID = glCreateProgram();
	glAttachShader(ID, vShader);
	glAttachShader(ID, fShader);
	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if(!success){
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "FAILED TO LINK SHADERS\n " << infoLog << '\n'; 
	}

	glDeleteShader(vShader);
	glDeleteShader(fShader);
}


string Shader::parseShader(const string &path){
	std::ifstream file(path);

	if(!file.is_open()){
		std::cout << "Failed to open a shader file\n";
		return "";
	}
	
	string line;
	string shaderSource;
		
	while(std::getline(file, line)){
		shaderSource += line + '\n';
	}

	//std::cout << shaderSource << "\n";
	return shaderSource;
}

void Shader::use(){
	glUseProgram(ID);
}

void Shader::setFloat(const char* name, float value){
	glUniform1f(glGetUniformLocation(ID, name), value);
}
void Shader::setFloat(const std::string& name, float value){
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setInt(const char* name, int value){
	glUniform1i(glGetUniformLocation(ID, name), value);
}

void Shader::setInt(const std::string& name, int value){
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const char *name, const glm::mat4& matrix){
	glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setVec3(const char* name, const glm::vec3& vec, int count){
	glUniform3fv(glGetUniformLocation(ID, name), count, &vec[0]);
}
void Shader::setVec3(const std::string& name, const glm::vec3& vec, int count){
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), count, &vec[0]);
}

void Shader::setVec4(const char* name, const glm::vec4& vec, int count){
	glUniform4fv(glGetUniformLocation(ID, name), count, &vec[0]);
}
