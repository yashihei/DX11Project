#include "DemoScene.h"

#include "DirectXTK/SimpleMath.h"
#include "Effect.h"
#include "Model.h"
#include "InputManager.h"
#include "Camera.h"

using namespace DirectX::SimpleMath;

DemoScene::DemoScene(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext) :
	m_device(device), m_deviceContext(deviceContext)
{
	m_effect = std::make_shared<Effect>(m_device, m_deviceContext);
	m_model = std::make_shared<Model>(m_device, m_deviceContext, m_effect);
	m_model->createFromPmx("assets/alicia/Alicia_solid.pmx");
	m_inputManager = std::make_shared<InputManager>();
	m_camera = std::make_shared<Camera>(Vector3::Zero, Vector3::Zero, 800.0f / 600.0f);
}

Scene* DemoScene::update()
{
	m_inputManager->update();
	if (m_inputManager->isPressedButton1())
		m_angle += 0.01f;
	if (m_inputManager->isPressedButton2())
		m_angle -= 0.01f;

	m_camera->pos = Vector3(0, 15, -15);
	m_camera->lookAt = Vector3(0, 15, 0);

	return this;
}

void DemoScene::draw()
{
	Matrix world, view, proj;
	Vector3 lightDir = { 0, 0, 1 };
	Vector4 diffuseColor = { 1, 1, 1, 1 };

	world = DirectX::XMMatrixRotationY(m_angle);

	view = m_camera->getViewMat();
	proj = m_camera->getProjMat();

	//�]�u
	world = XMMatrixTranspose(world);
	view = XMMatrixTranspose(view);
	proj = XMMatrixTranspose(proj);

	m_effect->setParam(world, view, proj, lightDir, diffuseColor);
	m_model->draw();
}
