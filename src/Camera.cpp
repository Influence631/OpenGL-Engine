#include <Camera.hpp>
#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <glm_print.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 worldUp, float pitch, float yaw) : Front(glm::vec3(0.0f, 0.0f, -1.0f)){
	Position = position;
	Pitch = pitch;
	Yaw = yaw;
	WorldUp = worldUp;
	
	//std::cout << "This is the front in constructor - " << Front << "\n";
	//std::cout << "WORLD UP - " << WorldUp << "\n";
	//std::cout << "Front - " << Front << "\n";
	//std::cout << "PITCH " << Pitch << " YAW " << Yaw << "\n";

	updateVectors();
}

glm::mat4 Camera::getViewMatrix(){
	return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime){
	//std::cout << "The input received is " << direction << " Delta time is " << deltaTime << "\n";
	//std::cout << "Front is " << Front << "\n";
	if(direction == FORWARD)
		Position += Front * Speed * deltaTime;
	if(direction == BACKWARD)
		Position -= Front * Speed * deltaTime;
	if(direction == LEFT)
		Position -= Right * Speed * deltaTime;
	if(direction == RIGHT)
		Position += Right * Speed * deltaTime;
	
	//std::cout << "Position from camera class " << Position << "\n";

}

void Camera::ProcessMouse(float xOffset, float yOffset){
	Pitch += yOffset * Sensitivity;
	Yaw += xOffset * Sensitivity;

	if(Pitch > 89.0f){
		Pitch = 89.0f;
	}
	else if (Pitch < -89.0f){
		Pitch = -89.0f;
	}

	updateVectors();
}

void Camera::ProcessScroll(float yOffset){
	Fov -= (yOffset);

	if(Fov > 120.0f){
		Fov = 120.0f;
	}
	else if(Fov < 1.0f){
		Fov = 1.0f;
	}
}
void Camera::updateVectors(){
	glm::vec3 front;

	front.x = cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
	front.y = sin(glm::radians(Pitch));
	front.z = cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
	
	Front = glm::normalize(front);
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}
