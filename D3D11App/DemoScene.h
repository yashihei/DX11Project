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
class DebugPlayer;

class DemoScene : public Scene {
public:
	DemoScene(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
	Scene* update() override;
	void draw() override;
private:
	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_deviceContext;
	ModelPtr m_model, m_sky, m_fighter, m_sphere;
	ComPtr<ID3D11ShaderResourceView> m_texture;
	Sprite2DPtr m_sprite;
	BillboardPtr m_billboard;
	InputManagerPtr m_inputManager;
	AudioManagerPtr m_audioManager;
	CameraPtr m_camera;
	std::shared_ptr<DebugPlayer> m_playerA, m_playerB;
	float m_volume = 0;
};
