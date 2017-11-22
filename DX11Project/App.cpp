// -----------------------------------------------
// File : App.cpp
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#include "App.h"

#include "DemoScene.h"
#include "Window.h"
#include "Graphics.h"
#include "InputManager.h"
#include "AudioManager.h"
#include "FPSManager.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx11.h>
#include <DirectXTK/CommonStates.h>

App::~App()
{
	ImGui_ImplDX11_Shutdown();
}

void App::run()
{
	const std::string titleText = "SP4RK! [DX11]";
	const int answer = MessageBox(NULL, "フルスクリーンで起動しますか？", titleText.c_str(), MB_YESNO | MB_ICONQUESTION);
	const bool fullScreen = (answer == IDYES) ? true : false;

	MSG msg = {};

	try {
		m_window = std::make_shared<Window>(titleText, 1280, 720, fullScreen);
		m_graphics = std::make_shared<Graphics>(m_window->getWidth(), m_window->getHeight(), m_window->getHandle(), fullScreen);
		m_inputManager = std::make_shared<InputManager>();
		m_audioManager = std::make_shared<AudioManager>();
		m_states = std::make_shared<DirectX::CommonStates>(m_graphics->getDevice().Get());
		m_fpsManager = std::make_shared<FPSManager>();
		m_currentScene = std::make_shared<DemoScene>(m_graphics->getDevice(), m_graphics->getDeviceContext(), m_inputManager, m_audioManager, m_states);

		ImGui_ImplDX11_Init(m_window->getHandle(), m_graphics->getDevice().Get(), m_graphics->getDeviceContext().Get());

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

	ImGui::Begin("DebugPanel");
	ImGui::Text("FPS : %2.1f", m_fpsManager->getFps());
	ImGui::End();

	m_graphics->beginScene();
	m_currentScene->draw();
	ImGui::Render();
	m_graphics->endScene();

	m_fpsManager->sync(60);
}
