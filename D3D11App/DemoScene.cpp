#include "DemoScene.h"

#include "DirectXTK/SimpleMath.h"
#include "Model.h"
#include "InputManager.h"
#include "AudioManager.h"
#include "Camera.h"

using namespace DirectX::SimpleMath;

DemoScene::DemoScene(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext) :
	m_device(device), m_deviceContext(deviceContext)
{
	m_model = std::make_shared<Model>(m_device, m_deviceContext);
	//m_model->createFromPmx("assets/1go/おんだ式ハッカドール1号_v1.00.pmx");
	m_model->createFromPmx("assets/alicia/Alicia_solid.pmx");
	//m_model->createFromPmx("assets/aichan/kizunaai.pmx");
	m_inputManager = std::make_shared<InputManager>();
	m_audioManager = std::make_shared<AudioManager>();
	m_camera = std::make_shared<Camera>(Vector3::Zero, Vector3::Zero, Vector3::Up, 800.0f / 600.0f);

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
}
