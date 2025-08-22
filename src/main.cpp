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
#include <texture.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow*, double xOffset, double yOffset);
void processInput(GLFWwindow* window);

const unsigned int screen_width = 1420;
const unsigned int screen_height = 920;

Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));

bool includeDirectionalLight = true;
bool includePointLights = false;
bool includeFlashLight = true;

float nrActivePointLights = 4;
const glm::vec3 pointLightPositions[] ={
	glm::vec3(3.2f, 7.0f, 4.3f),
	glm::vec3(2.4f, -2.0f, 7.0f),
	glm::vec3(7.0f, 5.2f, -2.0f),
	glm::vec3(12.0f, 9.0f, 10.0f)
};

glm::vec3 directionalLight = glm::vec3(0.2f, -1.3f, -0.3f);

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

	//make a function for loading textures
	unsigned int box_diffuse_map = load_texture("../assets/diffuse_map.png");
	unsigned int box_specular_map = load_texture("../assets/specular_map.png");
	unsigned int face_texture = load_texture("../assets/awesomeface.png", true);

	Shader shader = Shader("../src/lightedObject.vert", "../src/lightedObject.frag");
	shader.use();

	//set the textures & material maps
	shader.setInt("material.diffuse", 0);
	shader.setInt("material.specular", 1);
	shader.setInt("face", 2);
	
	//material
	shader.setFloat("material.shininess", 32);
	
	//set the light attributes
	//direcitonal light
	if(includeDirectionalLight){
		shader.setVec3("directionalLight.direction", directionalLight);
		shader.setVec3("directionalLight.ambient", glm::vec3(0.12f, 0.107f, 0.1201f));
		shader.setVec3("directionalLight.diffuse", glm::vec3(0.35f, 0.351f, 0.453512f));
		shader.setVec3("directionalLight.specular", glm::vec3(0.2f));
	}
	
	if(includePointLights){
		shader.setInt("nrActivePointLights", nrActivePointLights);
		std::cout << "Active lights " << nrActivePointLights << "\n" ; 
		for(int i = 0; i < nrActivePointLights; i++){
			shader.setVec3("pointLights[" + std::to_string(i) + "].position", pointLightPositions[i]);

			shader.setVec3("pointLights[" + std::to_string(i) + "].ambient", glm::vec3(0.1f));
			shader.setVec3("pointLights[" + std::to_string(i) + "].diffuse", glm::vec3(0.5f));
			shader.setVec3("pointLights[" + std::to_string(i) + "].specular", glm::vec3(0.7));

			shader.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0f);
			shader.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.07f);
			shader.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.0173275);
			
		}
	}
	//spotlight params
	//shader.setFloat("light.constant", 1.0f);
	//shader.setFloat("light.linear", 0.02245);
	//shader.setFloat("light.quadratic", 0.001975);
	//set the spot(flashlight) params;
	//shader.setVec3();
	
	//create shaders for lighting
	Shader lightSourceShader = Shader("../src/lightSource.vert", "../src/lightSource.frag");

	lightSourceShader.use();
	lightSourceShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
	
	glEnable(GL_DEPTH_TEST);
	while(!glfwWindowShouldClose(window)){
		processInput(window);
		
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//bind texture to the correct texture units so that the vshader sampler the correct textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, box_diffuse_map);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, box_specular_map);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, face_texture);
		
		//update deltatime
		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		
		//rendering
		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Fov), static_cast<float>(screen_width/screen_height), 0.1f, 100.0f);

		float radius = 0.0f;
		float slow = 1.0f;
		//glm::vec3 lightVector = lightPos + glm::vec3(cos(currentTime/slow),sin(currentTime/slow)*cos(currentTime/slow),sin(currentTime/slow)) * radius;	
		
		shader.use();
		//vert
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		//frag
		shader.setVec3("viewPos", camera.Position);
		
		glm::mat4 model;
		bindVAO(vao);
		for(int i=0; i < 10; i++){
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(i));
			model = glm::rotate(model, currentTime * i, glm::vec3(0.3f, 0.5f, 0.4f));
			shader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		//
		//draw the light source
		//
		lightSourceShader.use();
		lightSourceShader.setMat4("projection", projection);
		lightSourceShader.setMat4("view", view);
		
		bindVAO(lightCubeVAO);
		for(int i = 0; i < 4; i++){
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
			lightSourceShader.setMat4("model", model);
			
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		
		
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
