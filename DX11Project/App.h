// -----------------------------------------------
// File : App.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#include <memory>
#include "Fwd.h"
#include "PtrAlias.h"

class App {
public:
	App() = default;
	~App();
	void run();

	WindowPtr getWindow() const { return m_window; }
	GraphicsPtr getGraphics() const { return m_graphics; }
	InputManagerPtr getInputManager() const { return m_inputManager; }
	AudioManagerPtr getAudioManager() const { return m_audioManager; }
	AssetsMangerPtr getAssetsManager() const { return m_assetsManager; }
	CameraPtr getCamera() const { return m_camera; }
	LightParamPtr getLight() const { return m_light; }
	CommonStatesPtr getStates() const { return m_states; }
private:
	void frame();

	WindowPtr m_window;
	GraphicsPtr m_graphics;
	InputManagerPtr m_inputManager;
	AudioManagerPtr m_audioManager;
	AssetsMangerPtr m_assetsManager;
	CameraPtr m_camera;
	LightParamPtr m_light;
	CommonStatesPtr m_states;
	FPSManagerPtr m_fpsManager;
	ScenePtr m_currentScene;
};
