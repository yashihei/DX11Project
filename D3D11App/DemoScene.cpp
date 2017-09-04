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
	m_device(device), m_deviceContext(deviceContext)
{
	m_inputManager = std::make_shared<InputManager>();
	m_audioManager = std::make_shared<AudioManager>();

	m_states = std::make_unique<DirectX::CommonStates>(m_device.Get());
	m_deviceContext->OMSetDepthStencilState(m_states->DepthNone(), 0);
	m_deviceContext->OMSetBlendState(m_states->Additive(), 0, 0xFfFfFfFf);

	UINT vpCount = 1;
	D3D11_VIEWPORT vp = {};
	m_deviceContext->RSGetViewports(&vpCount, &vp);
	m_camera = std::make_shared<Camera>(Vector3::Zero, Vector3::Zero, Vector3::Up, vp.Width / vp.Height);
	m_camera->pos = Vector3(0, 0, -30);
	m_camera->lookAt = Vector3(0, 0, 0);

	m_alicia = std::make_shared<Model>(m_device, m_deviceContext);
	m_alicia->createFromPmx("assets/alicia/Alicia_solid.pmx");

	auto spriteTex = CreateShaderResourceViewFromFile(m_device, L"assets/circle.png");
	m_sprite = std::make_shared<Sprite>(m_device, m_deviceContext, spriteTex, m_camera);

	m_particles = std::make_shared<ActorManager<Particle>>();
}

Scene* DemoScene::update()
{
	m_inputManager->update();
	m_audioManager->update();

	m_particles->update();

	static int rate = 30;
	static float speed = 1.f;
	static float colorAry[] = { 1.f, 0.7f, 0.2f, 1.f };
	static float scale = 1.0f;
	static bool gravity = false;

	//num, speed, color
	ImGui::Begin("config");
	ImGui::SliderInt("Rate", &rate, 0, 100);
	ImGui::SliderFloat("Speed", &speed, 0, 1);
	ImGui::SliderFloat("Scale", &scale, 0, 2);
	ImGui::ColorEdit4("Color", colorAry);
	ImGui::Checkbox("Gravity", &gravity);
	ImGui::Text(("Particle Num:" + std::to_string(m_particles->size())).c_str());
	ImGui::End();

	for (int i = 0; i < rate; i++) {
		Quaternion rotate = Quaternion::CreateFromYawPitchRoll(Random(DirectX::XM_2PI), Random(DirectX::XM_2PI), Random(DirectX::XM_2PI));
		Vector3 vec(speed, 0, 0);
		vec = Vector3::Transform(vec, rotate);
		auto particle = std::make_shared<Particle>(m_sprite, Vector3::Zero, vec, Color(colorAry), scale, gravity);
		m_particles->add(particle);
	}

	return this;
}

void DemoScene::draw()
{
	m_particles->draw();
}
