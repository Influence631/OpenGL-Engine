#pragma once
#include <glad/glad.h>
#include <stb_image.h>
#include <iostream>

unsigned int load_texture(const char* path, bool flip = false){
	stbi_set_flip_vertically_on_load(flip);
	
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	//default settings;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	//mipmaps only exitst for minifying!
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char* data;
	int width, height, nrChannels;
	
	data = stbi_load(path, &width, &height, &nrChannels, 0);
	GLenum format;
	if(nrChannels == 1)
		format = GL_RED;
	else if(nrChannels == 3)
		format = GL_RGB;
	else if(nrChannels == 4)
		format = GL_RGBA;
	
	if(data){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else{
		std::cerr << "Failed to load texture - " << path << "\n";
		return EXIT_FAILURE;
	}
	stbi_image_free(data);
	return texture;
}
