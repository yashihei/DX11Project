#include "DemoScene.h"

#include "DirectXTK/SimpleMath.h"
#include "Model.h"
#include "InputManager.h"
#include "AudioManager.h"
#include "Camera.h"
#include "ShaderRV.h"
#include "Sprite2D.h"
#include "imgui/imgui.h"

#if defined(_DEBUG)
	#pragma comment(lib, "lib/imguiD.lib")
#else
	#pragma comment(lib, "lib/imgui.lib")
#endif

using namespace DirectX::SimpleMath;

DemoScene::DemoScene(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext) :
	m_device(device), m_deviceContext(deviceContext)
{
	m_model = std::make_shared<Model>(m_device, m_deviceContext);
	//m_model->createFromPmx("assets/1go/おんだ式ハッカドール1号_v1.00.pmx");
	//m_model->createFromPmx("assets/aichan/kizunaai.pmx");
	m_model->createFromPmx("assets/alicia/Alicia_solid.pmx");

	m_texture = CreateShaderResourceViewFromFile(m_device, L"./assets/mayuki_toka.png");
	m_sprite = std::make_shared<Sprite2D>(m_device, m_deviceContext, m_texture);
	m_camera = std::make_shared<Camera>(Vector3::Zero, Vector3::Zero, Vector3::Up, 800.0f / 600.0f);

	m_inputManager = std::make_shared<InputManager>();
	m_audioManager = std::make_shared<AudioManager>();
	m_audioManager->load("assets/pyonpyon.wav", "test");
}

Scene* DemoScene::update()
{
	m_inputManager->update();
	m_audioManager->update();

	if (m_inputManager->isPressedLeft())
		m_angle += 0.01f;
	if (m_inputManager->isPressedRight())
		m_angle -= 0.01f;

	m_camera->pos = Vector3(0, 15, -15);
	m_camera->lookAt = Vector3(0, 15, 0);

	if (m_inputManager->isClicledButton1()) {
		m_audioManager->play("test", true);
	}
	if (m_inputManager->isClicledButton2()) {
		m_audioManager->pause("test");
	}

	return this;
}

void DemoScene::draw()
{
	Matrix world, view, proj;

	world = DirectX::XMMatrixRotationY(m_angle);
	view = m_camera->getViewMat();
	proj = m_camera->getProjMat();

	//転置
	world = XMMatrixTranspose(world);
	view = XMMatrixTranspose(view);
	proj = XMMatrixTranspose(proj);

	m_model->draw(world, view, proj);

	static bool flag = false;
	if (flag)
		m_sprite->draw({ 0, 0 }, 0.0f, 1.0f, { 1.0f, 1.0f, 1.0f, 1.0f });

	ImGui::Begin("config");
	ImGui::SliderFloat("Rotation", &m_angle, 0, DirectX::XM_2PI);
	ImGui::Checkbox("View Sprite", &flag);
    ImGui::End();
}
