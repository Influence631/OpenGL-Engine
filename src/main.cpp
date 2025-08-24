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
void processInput(GLFWwindow* window, Shader& shader);

void manage_spot_light(Shader& shader);
void manage_directional_light(Shader& shader);
void manage_point_lights(Shader& shader);

const unsigned int screen_width = 1420;
const unsigned int screen_height = 920;

Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));

bool includeDirectionalLight = true;
bool includePointLight = false;
bool includeSpotLight = true;

const glm::vec3 vec0 = glm::vec3(0.0f);
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
	
	manage_spot_light(shader);
	manage_directional_light(shader);
	manage_point_lights(shader);

	Shader lightSourceShader = Shader("../src/lightSource.vert", "../src/lightSource.frag");
	lightSourceShader.use();
	lightSourceShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
	
	glEnable(GL_DEPTH_TEST);
	while(!glfwWindowShouldClose(window)){
		processInput(window, shader);
		
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

		//float radius = 0.0f;
		float slow = 10.0f;
		//glm::vec3 lightPosOffset = glm::vec3(cos(currentTime/slow),sin(currentTime/slow)*cos(currentTime/slow),sin(currentTime/slow)) * radius;	
		
		shader.use();
		//vert
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		//frag
		shader.setVec3("viewPos", camera.Position);
		
		if(includeSpotLight){
			shader.setVec3("spotLight.position", camera.Position);
			shader.setVec3("spotLight.direction", camera.Front);
		}
	
		glm::mat4 model;
		bindVAO(vao);
		for(int i=0; i < 10; i++){
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(i));
			model = glm::rotate(model, currentTime * i / slow, glm::vec3(0.3f, 0.5f, 0.4f));
			shader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		//
		//draw the light source
		//
		lightSourceShader.use();
		lightSourceShader.setMat4("projection", projection);
		lightSourceShader.setMat4("view", view);
		
		if(includePointLight){
			bindVAO(lightCubeVAO);
			for(int i = 0; i < 4; i++){
				model = glm::mat4(1.0f);
				model = glm::translate(model, pointLightPositions[i]);
				model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
				lightSourceShader.setMat4("model", model);
			
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
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

void processInput(GLFWwindow* window, Shader& shader){
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
		glfwSetWindowShouldClose(window, true);
	}
	if(glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS){
		includeSpotLight = !includeSpotLight;
		manage_spot_light(shader);
	}
	if(glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS){
		includePointLight = !includePointLight;
		manage_point_lights(shader);

	}
	if(glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS){
		includeDirectionalLight = !includeDirectionalLight;
		manage_directional_light(shader);
	}
	//camera movement
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

void manage_directional_light(Shader& shader){
	shader.use();
	if(includeDirectionalLight){
		shader.setVec3("directionalLight.direction", directionalLight);
		shader.setVec3("directionalLight.ambient", glm::vec3(0.1));
		shader.setVec3("directionalLight.diffuse", glm::vec3(0.4));
		shader.setVec3("directionalLight.specular", glm::vec3(0.2f));
	}else{
		shader.setVec3("directionalLight.ambient", vec0);
		shader.setVec3("directionalLight.diffuse", vec0);
		shader.setVec3("directionalLight.specular", vec0);
	}
}

void manage_point_lights(Shader& shader){
	shader.use();
	if(includePointLight){
		shader.setInt("nrActivePointLights", nrActivePointLights);
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
	else{
		shader.setInt("nrActivePointLights", 0);
	}
}

void manage_spot_light(Shader& shader){
	shader.use();
	if(includeSpotLight){
		shader.setVec3("spotLight.ambient", glm::vec3(0.1f));
		shader.setVec3("spotLight.diffuse", glm::vec3(0.6f));
		shader.setVec3("spotLight.specular", glm::vec3(1.0f));
		
		shader.setFloat("spotLight.innerCutOff", glm::cos(glm::radians(12.5f)));
		shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(14.5f)));
		
		shader.setFloat("spotLight.constant", 1.0f);
		shader.setFloat("spotLight.linear", 0.09f);
		shader.setFloat("spotLight.quadratic", 0.032f);
	}else{
		shader.setVec3("spotLight.ambient", vec0);
		shader.setVec3("spotLight.diffuse", vec0);
		shader.setVec3("spotLight.specular", vec0);
		
		shader.setFloat("spotLight.innerCutOff", glm::cos(glm::radians(0.0f)));
		shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(0.0f)));
		
		//we need to set any of these so we dont divide by 0 in the shader if these are defaulted to 0;
		shader.setFloat("spotLight.constant", -1.0f);
		//shader.setFloat("spotLight.linear", -1.0f);
		//shader.setFloat("spotLight.quadratic", -1.0f);
	}
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
