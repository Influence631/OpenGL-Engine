#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

unsigned int createVAO();
unsigned int createVBO();
unsigned int createEBO();

void bindVBO(unsigned int vbo);
void bindEBO(unsigned int ebo);
void bindVAO(unsigned int vao);

void sendDataVBO(int size, const float *data, GLenum usage = GL_STATIC_DRAW);
void sendDataEBO(unsigned int size, const unsigned int *data, GLenum usage = GL_STATIC_DRAW);


