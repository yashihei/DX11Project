// -----------------------------------------------
// File : App.cpp
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#include "App.h"

#include "DemoScene.h"
#include "Graphics.h"
#include "InputManager.h"
#include "AudioManager.h"
#include "FPSManager.h"
#include <DirectXTK/Keyboard.h>
#include <DirectXTK/Mouse.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx11.h>
#include <DirectXTK/CommonStates.h>

App::App() : m_hWnd(NULL), m_hInstance(GetModuleHandle(NULL)), m_ScreenWidth(1280), m_ScreenHeight(720), m_appName("SP4RK! [DX11]") {}

App::~App()
{
	ImGui_ImplDX11_Shutdown();
}

void App::run()
{
	const int answer = MessageBox(NULL, "フルスクリーンで起動しますか？", m_appName.c_str(), MB_YESNO | MB_ICONQUESTION);
	const bool fullScreen = (answer == IDYES) ? true : false;

	if (!registerWndClass())
		return;
	if (!initWindow(fullScreen))
		return;

	MSG msg = {};

	try {
		m_graphics = std::make_shared<Graphics>(m_ScreenWidth, m_ScreenHeight, m_hWnd, fullScreen);
		m_inputManager = std::make_shared<InputManager>();
		m_audioManager = std::make_shared<AudioManager>();
		m_states = std::make_shared<DirectX::CommonStates>(m_graphics->getDevice().Get());
		m_currentScene = std::make_shared<DemoScene>(m_graphics->getDevice(), m_graphics->getDeviceContext(), m_inputManager, m_audioManager, m_states);
		m_fpsManager = std::make_shared<FPSManager>();

		ImGui_ImplDX11_Init(m_hWnd, m_graphics->getDevice().Get(), m_graphics->getDeviceContext().Get());

		while (msg.message != WM_QUIT) {
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			} else {
				frame();
			}
		}
	} catch (const std::exception& error) {
		MessageBox(NULL, error.what(), "Exception", MB_OK | MB_ICONERROR);
	} catch (...) {
		MessageBox(NULL, "Unknown error", "Error", MB_OK | MB_ICONERROR);
	}
}

void App::frame()
{
	ImGui_ImplDX11_NewFrame();

	//Scene change check (State pattern)
	const auto nextScene = m_currentScene->update();
	if (m_currentScene.get() != nextScene) {
		m_currentScene.reset(nextScene);
	}

	m_inputManager->update();
	m_audioManager->update();

	m_graphics->beginScene();
	m_currentScene->draw();
	ImGui::Render();
	m_graphics->endScene();

	//m_fpsManager->sync(60);
}

bool App::registerWndClass()
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(m_hInstance, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_appName.c_str();
	wc.hIconSm = LoadIcon(m_hInstance, IDI_APPLICATION);

	if (RegisterClassEx(&wc) == 0)
		return false;

	return true;
}

bool App::initWindow(bool fullScreen)
{
	DWORD style;
	RECT rc = { 0, 0, m_ScreenWidth, m_ScreenHeight };

	if (fullScreen) {
		style = WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP;
	} else {
		style = WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_THICKFRAME);
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	}

	m_hWnd = CreateWindow(
		m_appName.c_str(),
		m_appName.c_str(),
		style,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		NULL,
		NULL,
		m_hInstance,
		NULL
	);

	if (!m_hWnd)
		return false;

	ShowWindow(m_hWnd, SW_SHOWNORMAL);
	//ShowCursor(false);

	return true;
}

LRESULT CALLBACK App::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ImGui_ImplDX11_WndProcHandler(hWnd, msg, wParam, lParam);

	switch (msg) {
	case WM_ACTIVATEAPP:
		DirectX::Mouse::ProcessMessage(msg, wParam, lParam);
		DirectX::Keyboard::ProcessMessage(msg, wParam, lParam);
		return 0;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			PostQuitMessage(0);
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		DirectX::Keyboard::ProcessMessage(msg, wParam, lParam);
		return 0;
	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		DirectX::Mouse::ProcessMessage(msg, wParam, lParam);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}
