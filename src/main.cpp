#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "buffers.hpp"
#include "Shader.hpp"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Camera.hpp>
#include <glm_print.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow*, double xOffset, double yOffset);
void processInput(GLFWwindow* window);

const unsigned int screen_width = 1420;
const unsigned int screen_height = 920;
Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));
const glm::vec3 lightPos = glm::vec3(0.0f, -1.0f, 3.0f);

float lastX = screen_width / 2;
float lastY = screen_height / 2;
bool firstMove = true;

float deltaTime = 0.0f;
float lastTime = 0.0f;

float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
 
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f
};

const glm::vec3 cubePositions[] = {
	glm::vec3(0.0f, 0.5f, 0.0f),
	glm::vec3(2.0f, 0.5f, -2.0f),
	glm::vec3(-1.0f, -2.5f, 1.0f),
	glm::vec3(-10.0f, 10.5f, -10.0f),
};

//const unsigned int indices[] = {
	//															0, 1, 2,
		//														2, 1, 3
			//												 };

int main(int argc, char** argv){
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window = glfwCreateWindow(screen_width, screen_height, "Custom Enginka", nullptr, nullptr);
	if (window == nullptr){
		std::cout << " Failed to initialise window\n";
		glfwTerminate();
		return EXIT_FAILURE;
	}
	//std::cout << "The window has been created\n";
	glfwMakeContextCurrent(window);
	
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		std::cout << "Failed to initialise GLAD\n";
		return EXIT_FAILURE;
	}

	glViewport(0, 0, screen_width, screen_height);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	

	//the normal cube
	unsigned int vao = createVAO();
	unsigned int vbo = createVBO();
	//unsigned int ebo = createEBO();

	//std::cout << "VAO " << vao << "VBO - " <<  vbo <<  "EBO -  " << ebo << "\n";	

	bindVAO(vao);
	bindVBO(vbo);
	//bindEBO(ebo);
	
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  
	sendDataVBO(sizeof(vertices), vertices);
	//sendDataEBO(sizeof(indices), indices);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	//the light source cube
	unsigned int lightCubeVAO = createVAO();
	bindVAO(lightCubeVAO);
	bindVBO(vbo);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	unsigned int brick_texture;
	glGenTextures(1, &brick_texture);
	glBindTexture(GL_TEXTURE_2D, brick_texture);
	//set texture wrapping params
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	//set texture filtering params
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//set mipmap + texutre filtering params
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	
	//stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	unsigned char *data = stbi_load("../assets/brick.jpg", &width, &height, &nrChannels, 0);
	if(data){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else{
		std::cerr << "Failed to load brick image\n";
		return EXIT_FAILURE;
	}
	stbi_image_free(data);


	unsigned int face_texture;
	glGenTextures(1, &face_texture);
	glBindTexture(GL_TEXTURE_2D, face_texture);
	//set parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//load the image
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("../assets/awesomeface.png", &width, &height, &nrChannels, 0);
	if(data){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else{
		std::cout << "Failed to load face texture\n";
		return EXIT_FAILURE;
	}
	stbi_image_free(data);

	Shader shader = Shader("../src/lightedObject.vert", "../src/lightedObject.frag");
	shader.use();
	shader.setInt("brick", 0);
	shader.setInt("face", 1);
	
	shader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setVec3("lightPos", lightPos);
	//set the material
	shader.setVec3("material.ambient", glm::vec3(0.0215f, 0.1745f, 0.0215f));
	shader.setVec3("material.diffuse", glm::vec3(0.07568f, 0.61424f, 0.07568f));
	shader.setVec3("material.specular", glm::vec3(0.633f, 0.727811f, 0.633f));
	shader.setFloat("material.shininess", 32);
	
	//create shaders for lighting
	Shader lightSourceShader = Shader("../src/lightSource.vert", "../src/lightSource.frag");

	lightSourceShader.use();
	lightSourceShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
	
	glEnable(GL_DEPTH_TEST);
	while(!glfwWindowShouldClose(window)){
		processInput(window);
		
		glClearColor(0.10f, 0.60f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//bind texture to the correct texture units so that the vshader sampler the correct textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, brick_texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, face_texture);
		
		//update deltatime
		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		//bind textures to corresponding texture units:
	  glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, brick_texture);
		//rendering
		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Fov), static_cast<float>(width/height), 0.1f, 100.0f);

		shader.use();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		shader.setVec3("viewPos", camera.Position);
		
		bindVAO(vao);
		glm::mat4 model;
		for(int i=0; i < 4; i++){
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(i));
			model = glm::rotate(model, currentTime * i, glm::vec3(0.3f, 0.5f, 0.4f));
			shader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		//glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//
		//draw the light source
		//
		lightSourceShader.use();
		lightSourceShader.setMat4("projection", projection);
		lightSourceShader.setMat4("view", view);

		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		lightSourceShader.setMat4("model", model);

		bindVAO(lightCubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &vao);

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window){
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  	camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  	camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  	camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  	camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
	//std::cout << "THE position is: X  " << camera.Position  << "\n"; 
}


void mouse_callback(GLFWwindow* window, double xPos, double yPos){
	if(firstMove){
		xPos = lastX;
		yPos = lastY;
		firstMove = false;
	}
	float xOffset = static_cast<float>(xPos - lastX);
	float yOffset = static_cast<float>(lastY - yPos);
	lastX = xPos;
	lastY = yPos;

	//std::cout << " PITCH "<< camera.Pitch << " YAW " << camera.Yaw << "\n";
	camera.ProcessMouse(xOffset, yOffset);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset){
	camera.ProcessScroll(static_cast<float>(yOffset));
}
