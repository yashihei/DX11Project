#pragma once

#include <Windows.h>
#include <string>
#include <memory>

#include "Graphics.h"
#include "Scene.h"

class App {
public:
	App();
	~App();
	void run();
private:
	bool registerWndClass();
	bool initWindow(bool fullScreen);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
	HWND m_hWnd;
	HINSTANCE m_hInstance;
	const int m_ScreenWidth, m_ScreenHeight;
	std::string m_appName;
	//graphics, inputMgr
	std::shared_ptr<Graphics> m_graphics;
	std::shared_ptr<Scene> m_scene;
};
