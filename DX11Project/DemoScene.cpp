#include "DemoScene.h"

#include "Model.h"
#include "InputManager.h"
#include "AudioManager.h"
#include "Camera.h"
#include "ShaderRV.h"
#include "Sprite.h"
#include "Sprite2D.h"
#include "imgui/imgui.h"
#include "Random.h"

DemoScene::DemoScene(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext) :
	m_device(device), m_deviceContext(deviceContext), m_rotation(0)
{
	m_inputManager = std::make_shared<InputManager>();
	m_audioManager = std::make_shared<AudioManager>();
	m_states = std::make_unique<DirectX::CommonStates>(m_device.Get());

	UINT vpCount = 1;
	D3D11_VIEWPORT vp = {};
	m_deviceContext->RSGetViewports(&vpCount, &vp);
	m_camera = std::make_shared<Camera>(Vector3::Zero, Vector3::Zero, Vector3::Up, vp.Width / vp.Height);
	m_camera->pos = Vector3(0, 0, -30);
	m_camera->lookAt = Vector3(0, 0, 0);

	m_alicia = std::make_shared<Model>(m_device, m_deviceContext);
	m_alicia->createFromPmx("assets/alicia/Alicia_solid.pmx");
}

Scene* DemoScene::update()
{
	m_inputManager->update();
	m_audioManager->update();

	//--------------------------------------------------
	// camera controll
	if (m_inputManager->isPressedLeft()) {
		m_rotation += 0.03f;
	}
	else if (m_inputManager->isPressedRight()) {
		m_rotation -= 0.03f;
	}
	Quaternion rotate = Quaternion::CreateFromYawPitchRoll(m_rotation, 0, 0);
	m_camera->pos = Vector3::Transform(Vector3(0, 0, -30), rotate);

	return this;
}

void DemoScene::draw()
{
}
