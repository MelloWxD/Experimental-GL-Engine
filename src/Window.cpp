#include "..\Header\Window.h"



Window::Window()
{
}

Window::Window(int x, int y, const char* name)
{
	_size = v2(x, y);
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(x, y, name, NULL, NULL);
	if (window == nullptr)
	{
		assert(false && "Failed to create GLFW Window");
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	
}

bool Window::Running()
{
	if (glfwWindowShouldClose(window))
	{
		return false;
	}
	return true;
}

Window::~Window()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}