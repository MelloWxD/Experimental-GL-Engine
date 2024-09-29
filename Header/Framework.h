#pragma once
#include "Constants.h"
#include "Window.h"
#include "InputHandle.h"
#include "Renderer.h"
#include "AssetManager.h"
#include "Editor.h"

class Framework
{
public:
	
	Framework()
	{
		pWindow = new Window(SCREEN_RES_X, SCREEN_RES_Y, WINDOW_NAME);
		InitGL();
		pAssetMan = new AssetManager();		

		pEditorWindow = new Editor(pWindow, pAssetMan);

		pRenderer = new Renderer(pWindow, pAssetMan, pEditorWindow);	

		pInputHandle = new InputHandler(pWindow->window, this, pRenderer->pCamera, pEditorWindow);
		
		_bRunning = true;
		pEditorWindow->_pRenderer = pRenderer;
		pEditorWindow->_pFramework = this;
	}

	void InitGL();
	void RunApp();
	bool _bRunning = false;

	float fps_start;
	float fps_current;
	int fpsInLastSecond = 0;

	float _currentFrameTime = 0.f;
	float _currentDeltaTime = 0.f;
	float _lastFrameTime = 0.f;

	Window* pWindow;
	Renderer* pRenderer;
	Editor* pEditorWindow;
	AssetManager* pAssetMan;
	InputHandler* pInputHandle;
};

