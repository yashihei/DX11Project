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
	m_model = std::make_shared<Model>(m_device, m_deviceContext);
	m_sky = std::make_shared<Model>(m_device, m_deviceContext);
	m_sky->createFromPmx("assets/sky/Dome_X501.pmx");
	m_fighter = std::make_shared<Model>(m_device, m_deviceContext);
	m_fighter->createFromPmx("assets/fighter/eurofighter_mini.pmx");
	m_sphere = std::make_shared<Model>(m_device, m_deviceContext);
	m_sphere->createFromPmx("assets/sphere/sphere_tex.pmx");

	m_camera = std::make_shared<Camera>(Vector3::Zero, Vector3::Zero, Vector3::Up, 800.0f / 600.0f);
	m_camera->pos = Vector3(0, 8, -25);
	m_camera->lookAt = Vector3(0, 0, 0);

	//create 2d sprite
	m_inputManager = std::make_shared<InputManager>();
	m_audioManager = std::make_shared<AudioManager>();

	m_playerA = std::make_shared<DebugPlayer>(m_fighter, m_sphere, m_camera, "PlayerA");
	m_playerB = std::make_shared<DebugPlayer>(m_fighter, m_sphere, m_camera, "PlayerB");
}

Scene* DemoScene::update()
{
	m_inputManager->update();
	m_audioManager->update();

	m_playerA->update();
	m_playerB->update();

	return this;
}

void DemoScene::draw()
{
	m_sky->draw(Matrix::CreateScale(5.0f), m_camera->getViewMat().Transpose(), m_camera->getProjMat().Transpose());

	float distance = Vector3::Distance(m_playerA->getPos(), m_playerB->getPos());
	ImGui::Begin("DebugInfo");
	if (distance < m_playerA->getRadius() + m_playerB->getRadius()) {
		ImGui::Text("Hit!");
	}
	ImGui::Text("Distance: %f", distance);
	ImGui::End();

	m_playerA->draw();
	m_playerB->draw();
}
