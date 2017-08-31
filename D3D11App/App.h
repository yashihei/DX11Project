#pragma once

#include <Windows.h>
#include <string>
#include <memory>
#include "Fwd.h"

class App {
public:
	App();
	~App();
	void run();
private:
	void frame();
	bool registerWndClass();
	bool initWindow(bool fullScreen);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
	HWND m_hWnd;
	HINSTANCE m_hInstance;
	const int m_ScreenWidth, m_ScreenHeight;
	std::string m_appName;
	GraphicsPtr m_graphics;
	ScenePtr m_scene;
};
