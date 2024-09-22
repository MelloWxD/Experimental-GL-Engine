#pragma once
#include "Constants.h"
class Camera;

class InputHandler
{
public:
	InputHandler(GLFWwindow* pWindow, Camera* cam);
	
	void handleMouse();
	void Update(float dTime);
	
	void CheckInputs(float dTime);

	const float MouseSensetivity = 0.1f;

	//For mouse tracking
	double lastX = -FLT_MAX;
	double lastY = -FLT_MAX;

	bool camLock = true;
	GLFWwindow* pWin;
	Camera* pCam;
};

