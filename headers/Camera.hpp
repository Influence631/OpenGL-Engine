#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

enum Camera_Movement{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

//default variables
inline constexpr float PITCH = 0.0f;
inline constexpr float YAW = -90.0f;
inline constexpr float SPEED = 4.0f;
inline constexpr float SENSITIVITY = 0.05f;
inline constexpr float FOV = 45.0f;

class Camera{
	public:
	glm::vec3 Front;
	glm::vec3 Right;
	glm::vec3 Up;
	glm::vec3 WorldUp;
	glm::vec3 Position;
	
	float Speed = SPEED;
	float Sensitivity = SENSITIVITY;
	float Pitch = PITCH;
	float Yaw = YAW;
	float Fov = FOV;
	public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f), float pitch = PITCH, float yaw = YAW);

	glm::mat4 getViewMatrix();
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);
	void ProcessMouse(float xOffset, float yOffset);
	void ProcessScroll(float yOffset);

	private:
	void updateVectors();
	
};
