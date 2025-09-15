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

unsigned int load_skybox(const char* faces[], const string subdirectory = ""){
	stbi_set_flip_vertically_on_load(false);

	unsigned int texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	std::string path;
	int width, height, nrChannels;
	for(int i = 0; i < 6; i++){
		path = subdirectory + faces[i];
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
		GLenum format;
		if(nrChannels == 1)
			format = GL_RED;
		else if(nrChannels == 3)
			format = GL_RGB;
		else if(nrChannels == 4)
			format = GL_RGBA;
		if(data){
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else{
			std::cerr << "Failed to load the skybox at path : " << path << " \n"; 
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S , GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T , GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R , GL_CLAMP_TO_EDGE);

	return texture;
}
