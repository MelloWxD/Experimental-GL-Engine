#pragma once
#include "Constants.h"

class Camera;
class Editor;
class Framework;
class Renderer;

class InputHandler
{
public:
	InputHandler(GLFWwindow* pWindow,Framework* pfw, Camera* cam, Editor* pEdit);
	
	void handleMouse();
	void Update();
	
	const bool CheckKey(int keyNum, int glfw_check);

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	void CheckInputs();

	const float MouseSensetivity = 0.1f;

	//For mouse tracking
	double lastX = -FLT_MAX;
	double lastY = -FLT_MAX;
	bool toggle = false;

	int camLock = 0; // 0 - normal fps cam, 1 - mouse free but fps cam, 2 - locked fps cam free mouse
	GLFWwindow* pWin;
	Framework* pFw; // for delta time
	Renderer* pRenderer; // for delta time
	Camera* pCam;
	Editor* pEditor;
};

