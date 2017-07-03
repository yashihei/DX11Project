#include "DemoScene.h"

#include "Model.h"
#include "InputManager.h"
#include "AudioManager.h"
#include "Camera.h"
#include "ShaderRV.h"
#include "Sprite2D.h"
#include "Billboard.h"
#include "imgui/imgui.h"
#include "Actors.h"

#if defined(_DEBUG)
	#pragma comment(lib, "lib/imguiD.lib")
#else
	#pragma comment(lib, "lib/imgui.lib")
#endif

DemoScene::DemoScene(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext) :
	m_device(device), m_deviceContext(deviceContext)
{
	m_alicia = std::make_shared<Model>(m_device, m_deviceContext);
	m_alicia->createFromPmx("assets/alicia/Alicia_solid.pmx");

	m_camera = std::make_shared<Camera>(Vector3::Zero, Vector3::Zero, Vector3::Up, 800.0f / 600.0f);
	m_camera->pos = Vector3(0, 15, -15);
	m_camera->lookAt = Vector3(0, 15, 0);

	m_inputManager = std::make_shared<InputManager>();
	m_audioManager = std::make_shared<AudioManager>();
}

Scene* DemoScene::update()
{
	m_inputManager->update();
	m_audioManager->update();

	return this;
}

void DemoScene::draw()
{
	Vector3 rot;

	ImGui::Begin("config");
	float vRot[3] = { rot.x, rot.y, rot.z };
	ImGui::SliderFloat3("Rotation", vRot, 0, DirectX::XM_2PI, "%.3f");
	rot = Vector3(vRot);
	ImGui::End();

	Matrix world, view, proj;
	Vector3 lightDir = { 0, 0, 1 };
	Vector4 diffuseColor = { 1, 1, 1, 1 };

	world = Matrix::CreateFromYawPitchRoll(rot.y, rot.x, rot.z);
	view = m_camera->getViewMat();
	proj = m_camera->getProjMat();

	m_alicia->draw(world.Transpose(), view.Transpose(), proj.Transpose());
}
