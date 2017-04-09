#pragma once

#include <Windows.h>
#include <string>
#include <memory>

class D3D11App {
public:
	D3D11App();
	~D3D11App();
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
};
