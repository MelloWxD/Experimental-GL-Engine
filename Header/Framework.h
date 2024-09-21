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
		pWindow = new Window(SCREEN_RES_X, SCREEN_RES_Y, WINDOW_NAME);
		InitGL();
		pAssetMan = new AssetManager();
		pRenderer = new Renderer(pWindow, pAssetMan);
		pInputHandle = new InputHandler(pWindow->window, pRenderer->pCamera);

		_bRunning = true;
	}
	void InitGL();
	void RunApp();
	bool _bRunning = false;

	float fps_start;
	float fps_current;
	int fpsInLastSecond = 0;

	float _currentFrameTime = 0.f;
	float _lastFrameTime = 0.f;

	Window* pWindow;
	Renderer* pRenderer;
	AssetManager* pAssetMan;
	InputHandler* pInputHandle;
};

