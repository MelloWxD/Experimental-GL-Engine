#pragma once

class Window;
class Renderer;
class Framework;
class AssetManager;

class Editor
{
public:
	Editor(Window* pWin, AssetManager* pAM);
	~Editor();

	void Draw_Editor();

	void ToggleActive();
	Renderer* _pRenderer;
	Framework* _pFramework;
private:
	bool _enabled = true;
	void init_imgui();
	Window* _pWindow;
	AssetManager* pAssManager;
};

