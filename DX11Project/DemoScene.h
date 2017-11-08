// -----------------------------------------------
// File : DemoScene.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <boost/timer.hpp>
#include "DirectXTK/SimpleMath.h"
#include "Scene.h"
#include "Fwd.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX::SimpleMath;

class DemoScene : public Scene {
public:
	DemoScene(
		ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext,
		InputManagerPtr inputManager, AudioManagerPtr audioManager, CommonStatesPtr states);
	Scene* update() override;
	void draw() override;
private:
	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_deviceContext;
	CommonStatesPtr m_states;
	InputManagerPtr m_inputManager;
	AudioManagerPtr m_audioManager;
	CameraPtr m_camera;
	ModelPtr m_model;
	boost::timer m_timer;
};
