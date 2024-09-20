#pragma once
#include"Constants.h"
class Camera
{
public:
	Camera(glm::vec3 position = glm::vec3(0));
	
	

	void Update();
	
	m4 GetViewMatrix();
	glm::vec3 position;
	glm::vec3 camTarget;
	glm::vec3 Direction;

	glm::vec3 Up = glm::vec3(0.f, 1.f, 0.f);
	glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 camRight;
	glm::vec3 camFront;
	glm::vec3 camUp;

	float speed = 2.5f;

	float pitch = 0.f;
	float yaw = -90.f;
	
	float nearPlane = 0.1f;
	float farPlane = 1000.f;

	float _fov = 90.f;
	m4 camMatrix = m4(1.f);
	glm::mat4 view = glm::mat4(1.f);
};

