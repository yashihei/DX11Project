// -----------------------------------------------
// File : Window.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#include <Windows.h>
#include <string>

class Window {
public:
	Window(const std::string& titleText, int width, int height, bool fullScreen, bool cursor = true);
	HWND getHandle() const { return m_hWnd; }
	HINSTANCE getHInstance() const { return m_hInstance; }
	int getWidth() const { return m_width; }
	int getHeight() const { return m_height; }
private:
	bool registerWndClass();
	bool initWindow(bool fullScreen);
	HWND m_hWnd;
	HINSTANCE m_hInstance;
	std::string m_titleText;
	int m_width, m_height;
};
