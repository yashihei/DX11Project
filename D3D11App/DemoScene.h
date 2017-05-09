#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <boost/timer.hpp>
#include <deque>
#include <vector>
#include "DirectXTK/SimpleMath.h"
#include "Scene.h"
#include "Fwd.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX::SimpleMath;

class DemoScene : public Scene {
public:
	DemoScene(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
	Scene* update() override;
	void draw() override;
private:
	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_deviceContext;
	ModelPtr m_model, m_sky;
	ComPtr<ID3D11ShaderResourceView> m_texture;
	Sprite2DPtr m_sprite;
	InputManagerPtr m_inputManager;
	AudioManagerPtr m_audioManager;
	CameraPtr m_camera;
	boost::timer m_timer;
	std::deque<float> m_deltaTimes;
	Vector3 m_rotation;
	float m_volume = 0;
};
