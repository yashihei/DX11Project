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
private:
	void frame();
	WindowPtr m_window;
	GraphicsPtr m_graphics;
	InputManagerPtr m_inputManager;
	AudioManagerPtr m_audioManager;
	CommonStatesPtr m_states;
	FPSManagerPtr m_fpsManager;
	ScenePtr m_currentScene;
};
