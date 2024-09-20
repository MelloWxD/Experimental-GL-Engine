#pragma once
#include"Constants.h"



class Window
{
public:
	Window();
	Window(int x, int y, const char* name);
	
	bool Running();
	
	~Window();
	
	v2 _size;
	GLFWwindow* window;
};

