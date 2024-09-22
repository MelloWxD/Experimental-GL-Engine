#include "..\Header\Framework.h"

void Framework::InitGL()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		assert(false);
	}	glViewport(0, 0, pWindow->_size.x, pWindow->_size.y);
	glClearColor(0.07f, 0.13f, 0.17f, 1.f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(pWindow->window);
	fps_start = glfwGetTime();
}

void Framework::RunApp()
{
	// Clocks
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
	}
	if (!pWindow->Running()) // If terminated then set flag to false
	{
		_bRunning = false;
		return;
	}
	// else Do Frame Render
	
	pRenderer->Display();

	pInputHandle->Update(_currentFrameTime - _lastFrameTime);
	_lastFrameTime = _currentFrameTime;
	
}
