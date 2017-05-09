#include "DemoScene.h"

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

DemoScene::DemoScene(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext) :
	m_device(device), m_deviceContext(deviceContext)
{
	m_model = std::make_shared<Model>(m_device, m_deviceContext);
	//m_model->createFromPmx("assets/1go/おんだ式ハッカドール1号_v1.00.pmx");
	//m_model->createFromPmx("assets/aichan/kizunaai.pmx");
	m_model->createFromPmx("assets/alicia/Alicia_solid.pmx");

	m_sky = std::make_shared<Model>(m_device, m_deviceContext);
	m_sky->createFromPmx("assets/sky/Dome_X501.pmx");

	m_texture = CreateShaderResourceViewFromFile(m_device, L"./assets/mayuki_toka.png");
	m_sprite = std::make_shared<Sprite2D>(m_device, m_deviceContext, m_texture);

	m_camera = std::make_shared<Camera>(Vector3::Zero, Vector3::Zero, Vector3::Up, 800.0f / 600.0f);

	m_inputManager = std::make_shared<InputManager>();
	m_audioManager = std::make_shared<AudioManager>();
	m_audioManager->load("assets/pyonpyon.wav", "test");
	m_audioManager->play("test", true, m_volume);
}

Scene* DemoScene::update()
{
	m_inputManager->update();
	m_audioManager->update();

	//if (m_inputManager->isPressedLeft())
	//	m_angle += 0.01f;
	//if (m_inputManager->isPressedRight())
	//	m_angle -= 0.01f;

	m_camera->pos = Vector3(0, 15, -15);
	m_camera->lookAt = Vector3(0, 15, 0);

	if (m_inputManager->isClicledButton1())
		m_audioManager->play("test", true);
	if (m_inputManager->isClicledButton2())
		m_audioManager->pause("test");
	m_audioManager->setVolume("test", m_volume);

	m_deltaTimes.push_front(static_cast<float>(m_timer.elapsed()));
	if (m_deltaTimes.size() > 60)
		m_deltaTimes.pop_back();
	m_timer.restart();

	return this;
}

void DemoScene::draw()
{
	Matrix world, view, proj;

	world = Matrix::CreateFromYawPitchRoll(m_rotation.y, m_rotation.x, m_rotation.z);
	view = m_camera->getViewMat();
	proj = m_camera->getProjMat();

	//転置
	world = XMMatrixTranspose(world);
	view = XMMatrixTranspose(view);
	proj = XMMatrixTranspose(proj);

	m_sky->draw(Matrix::Identity, view, proj);
	m_model->draw(world, view, proj);

	static bool flag = false;
	if (flag)
		m_sprite->draw({ 0, 0 }, 0.0f, 0.5f, { 1.0f, 1.0f, 1.0f, 1.0f });

	ImGui::Begin("config");
	ImGui::SliderFloat("RotationX", &m_rotation.x, 0, DirectX::XM_2PI);
	ImGui::SliderFloat("RotationY", &m_rotation.y, 0, DirectX::XM_2PI);
	ImGui::SliderFloat("RotationZ", &m_rotation.z, 0, DirectX::XM_2PI);
	ImGui::SliderFloat("Volume", &m_volume, 0, 1.0f);
	ImGui::Checkbox("View Sprite", &flag);
    ImGui::End();

	std::vector<float> ary;
	for (auto& time : m_deltaTimes)
		ary.push_back(time);

	ImGui::Begin("Status");
	ImGui::PlotLines("DeltaTime", ary.data(), ary.size(), 0, "", 0, *ary.begin() * 2, ImVec2(0, 50));
	ImGui::End();
}
