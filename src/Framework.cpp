#include "..\Header\Framework.h"

void Framework::InitGL()
{
	gladLoadGL(); // init GLAD/OpenGL
	glViewport(0, 0, pWindow->_size.x, pWindow->_size.y);
	glClearColor(0.07f, 0.13f, 0.17f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(pWindow->window);
	fps_start = glfwGetTime();
}

void Framework::RunApp()
{
	_currentFrameTime = glfwGetTime();
	if (fps_current >= fps_start + 1.f)
	{
		std::string newName = WINDOW_NAME;
		newName += " - FPS: " + std::to_string(fpsInLastSecond);
		glfwSetWindowTitle(pWindow->window, newName.c_str());
		fpsInLastSecond = 0;
		fps_start = glfwGetTime();
	}
	else
	{
		fps_current = glfwGetTime();
		++fpsInLastSecond;
	}
	/// Should generally stick to pre render, render, input handle and response in that order
	if (!pWindow->Running()) // If terminated then set flag to false
	{
		_bRunning = false;
		return;
	}
	
	pRenderer->Display();

	pInputHandle->Update(_currentFrameTime - _lastFrameTime);
	_lastFrameTime = _currentFrameTime;
	
}
