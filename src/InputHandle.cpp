#include "..\Header\InputHandle.h"
#include "..\Header\Editor.h"
#include "..\Header\Camera.h"
#include "..\Header\Renderer.h"
#include "..\Header\Framework.h"



bool firstMouse = true;


InputHandler::InputHandler(GLFWwindow* pWindow, Framework* pfw, Camera* cam, Editor* pEdit)
{
	assert(pWindow != nullptr);
	pWin = pWindow;
	pCam = cam;
	pEditor = pEdit;
	pFw = pfw;
	pRenderer = pfw->pRenderer;
}

void InputHandler::handleMouse()
{
	double xpos, ypos;
	glfwGetCursorPos(pWin, &xpos, &ypos);
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	float yaw = pCam->yaw;
	float pitch = pCam->pitch;
	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	glm::vec3 front = glm::normalize(direction);

	pCam->yaw = yaw;
	pCam->pitch = pitch;
	pCam->Direction = direction;
	pCam->Front = front;

}

void InputHandler::Update()
{
	glfwPollEvents();
	CheckInputs();
}

const bool InputHandler::CheckKey(int keyNum, int glfw_check = GLFW_PRESS)
{
	return (glfwGetKey(pWin, keyNum) == glfw_check);
}
void InputHandler::CheckInputs()
{
	if (CheckKey(GLFW_KEY_ESCAPE)) // Close on ESC Press
	{
		glfwSetWindowShouldClose(pWin, true);
	}
	// Movement
	{
		float speed = pCam->speed * pFw->_currentDeltaTime;
		auto& camRef = *pCam;
		if (glfwGetKey(pWin, GLFW_KEY_W) == GLFW_PRESS)
		{
			camRef.position = camRef.position + speed * pCam->camFront;

		}
		if (glfwGetKey(pWin, GLFW_KEY_S) == GLFW_PRESS)
		{
			camRef.position -= speed * pCam->camFront;		

		}
		if (glfwGetKey(pWin, GLFW_KEY_A) == GLFW_PRESS)
		{
			camRef.position -= glm::normalize(glm::cross(pCam->camFront, pCam->Up)) * speed;			

		}
		if (glfwGetKey(pWin, GLFW_KEY_D) == GLFW_PRESS)
		{
			camRef.position += glm::normalize(glm::cross(pCam->camFront, pCam->Up)) * speed;		

		}
		if (glfwGetKey(pWin, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			camRef.position += speed * pCam->Up;			

		}
		if (glfwGetKey(pWin, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		{
			camRef.position -= speed * pCam->Up;			
		}
	}
	// Mouse
	if (camLock == 0 || camLock == 1)
	{
		handleMouse();
	}
	if (CheckKey(GLFW_KEY_CAPS_LOCK))
	{
		glfwWaitEventsTimeout(.7f);
		pRenderer->debug = !pRenderer->debug;		

	}
	if (CheckKey(GLFW_KEY_F))
	{
		(camLock > 2) ? camLock = 0 : ++camLock;
		if (camLock == 0)
		{
			glfwSetInputMode(pWin, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		}
		else
		{
			glfwSetInputMode(pWin, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		glfwWaitEventsTimeout(0.7f);

	}

	if (CheckKey(GLFW_KEY_TAB))
	{
		glfwWaitEventsTimeout(0.7f);
		pEditor->ToggleActive();			

	}
}