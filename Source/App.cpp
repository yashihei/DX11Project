// -----------------------------------------------
// File : App.cpp
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#include "App.h"

#define NOMINMAX
#include "DemoScene.h"
#include "Window.h"
#include "Graphics.h"
#include "InputManager.h"
#include "AudioManager.h"
#include "AssetsManager.h"
#include "FPSManager.h"
#include "Camera.h"
#include "LightParam.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx11.h>
#include <DirectXTK/CommonStates.h>
#include <cpptoml/cpptoml.h>

App::~App()
{
	ImGui_ImplDX11_Shutdown();
}

void App::run()
{
	auto config = cpptoml::parse_file("config.toml");

	const auto titleText = config->get_as<std::string>("title_text").value_or("");
	const auto width = config->get_as<int>("screen_width").value_or(1280);
	const auto height = config->get_as<int>("screen_height").value_or(720);
	const auto fullScreen = config->get_as<bool>("fullscreen").value_or(false);

	MSG msg = {};

	m_window = std::make_shared<Window>(titleText, width, height, fullScreen);
	m_graphics = std::make_shared<Graphics>(width, height, m_window->getHandle(), fullScreen);
	m_inputManager = std::make_shared<InputManager>();
	m_audioManager = std::make_shared<AudioManager>();
	m_assetsManager = std::make_shared<AssetsManager>();
	m_camera = std::make_shared<Camera>(Vector3::Zero, Vector3::Zero, Vector3::Up, static_cast<float>(width) / static_cast<float>(height));
	m_light = std::make_shared<LightParam>(Color(1.0f, 1.0f, 1.0f, 1.0f), Color(0.3f, 0.3f, 0.3f, 1.0f), Vector3(0.0f, -1.0f, 0.0f));
	m_states = std::make_shared<DirectX::CommonStates>(m_graphics->getDevice().Get());
	m_fpsManager = std::make_shared<FPSManager>();
	m_currentScene = std::make_shared<DemoScene>(this);

	ImGui_ImplDX11_Init(m_window->getHandle(), m_graphics->getDevice().Get(), m_graphics->getDeviceContext().Get());

	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {
			frame();
		}
	}
}

void App::frame()
{
	ImGui_ImplDX11_NewFrame();

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
