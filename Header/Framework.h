#pragma once
#include "Constants.h"
#include "Window.h"
#include "InputHandle.h"
#include "Renderer.h"
#include "AssetManager.h"

class Framework
{
public:
	
	Framework()
	{
		pWindow = new Window(SCREEN_RES_X, SCREEN_RES_Y, "I have been job hunting for 6+ months PLEASE GIMME SOMETHING");
		InitGL();
		pAssetMan = new AssetManager();
		pRenderer = new Renderer(pWindow, pAssetMan);
		pInputHandle = new InputHandler(pWindow->window, pRenderer->pCamera);

		_bRunning = true;
	}
	void InitGL();
	void RunApp();
	bool _bRunning = false;

	float _currentFrameTime = 0.f;
	float _lastFrameTime = 0.f;

	Window* pWindow;
	Renderer* pRenderer;
	AssetManager* pAssetMan;
	InputHandler* pInputHandle;
};

