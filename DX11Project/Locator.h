// -----------------------------------------------
// File : Locator.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#include "Fwd.h"
#include "PtrAlias.h"

class Locator {
public:
	Locator()
	{
		pLocator = this;
	}

	static void provideGraphics(GraphicsPtr graphics) { pLocator->m_graphics = graphics; }
	static void provideAudioManager(AudioManagerPtr audioManager) { pLocator->m_audioManager = audioManager; }
	static void provideInputManager(InputManagerPtr inputManager) { pLocator->m_inputManager = inputManager; }
	static void provideCamera(CameraPtr camera) { pLocator->m_camera = camera; }
	static void provideLight(LightParamPtr light) { pLocator->m_light = light; }
	static void provideCommonStates(CommonStatesPtr states) { pLocator->m_states = states; }

	static GraphicsPtr getGraphics() { return pLocator->m_graphics; }
	static AudioManagerPtr getAudioManager() { return pLocator->m_audioManager; }
	static InputManagerPtr getInputManager() { return pLocator->m_inputManager; }
	static CameraPtr getCamera() { return pLocator->m_camera; }
	static LightParamPtr getLight() { return pLocator->m_light; }
	static CommonStatesPtr getCommonStates() { return pLocator->m_states; }
private:
	static Locator* pLocator;

	GraphicsPtr m_graphics;
	AudioManagerPtr m_audioManager;
	InputManagerPtr m_inputManager;
	CameraPtr m_camera;
	LightParamPtr m_light;
	CommonStatesPtr m_states;
	//TODO:AssetManager
};
