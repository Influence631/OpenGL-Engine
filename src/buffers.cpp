#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <buffers.hpp>


unsigned int createVAO(){
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	return VAO;
}
unsigned int createVBO(){
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	return VBO;
}
unsigned int createEBO(){
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	return EBO;
}

void bindVBO(unsigned int VBO){
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
}

void bindVAO(unsigned int VAO){
	glBindVertexArray(VAO);
}

void bindEBO(unsigned int EBO){
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
}

void sendDataVBO(int size, const float* data, GLenum usage){
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}
void sendDataEBO(unsigned int size, const unsigned int* data, GLenum usage){
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
}

