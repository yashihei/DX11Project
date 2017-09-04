#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <boost/timer.hpp>
#include <deque>
#include <vector>
#include "DirectXTK/SimpleMath.h"
#include "DirectXTK/CommonStates.h"
#include "Scene.h"
#include "Fwd.h"
#include "Particle.h"

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
	std::unique_ptr<DirectX::CommonStates> m_states;
	InputManagerPtr m_inputManager;
	AudioManagerPtr m_audioManager;
	CameraPtr m_camera;
	ModelPtr m_alicia;
	SpritePtr m_sprite;
	ParticleManagerPtr m_particles;
	boost::timer m_timer;
};
