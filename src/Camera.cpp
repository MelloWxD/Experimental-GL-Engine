#include "..\Header\Camera.h"

Camera::Camera(glm::vec3 position)
{
	position = position; 
	view = glm::mat4(1.f);
	camTarget = glm::vec3(0);
	Direction = glm::vec3(0);
	camRight = glm::normalize(glm::cross(Up, Direction));
	
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	Front = glm::normalize(front);
	// also re-calculate the Right and Up vector
	camRight = glm::normalize(glm::cross(front, Up));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	camUp = glm::normalize(glm::cross(camRight, Front));
}
m4 Camera::GetViewMatrix()
{
	//this->view = glm::lookAt(this->position, this->position + this->Front, this->camUp);
	return this->view;
}
void Camera::Update()
{
	// Update matrix
	// Initializes matrices since otherwise they will be the null matrix
	glm::mat4 view = glm::mat4(1.0f);


	// Sets new camera matrix
	Direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	Direction.y = sin(glm::radians(pitch));
	Direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	Front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	Front.y = sin(glm::radians(pitch));
	Front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	camFront = glm::normalize(Front);
	// also re-calculate the Right and Up vector
	camRight = glm::normalize(glm::cross(camFront, Up));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	camUp = glm::normalize(glm::cross(camRight, Front));

	// Makes camera look in the right direction from the right position
	this->view = glm::lookAt(this->position, this->position + this->Front, this->Up);
	// Adds perspective to the scene
	
}

