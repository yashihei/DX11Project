#include "DemoScene.h"

#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

#include <fstream>

DemoScene::DemoScene(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext) :
	m_device(device), m_deviceContext(deviceContext)
{
	m_effect = std::make_shared<Effect>(m_device, m_deviceContext);
	m_model = std::make_shared<Model>(m_device, m_deviceContext);
}

Scene* DemoScene::update()
{
	m_angle += 0.01f;
	return this;
}

void DemoScene::draw()
{
	Matrix world, view, proj;
	Vector3 lightDir = { 0, 0, 1 };
	Vector4 diffuseColor = { 1, 1, 1, 1 };

	world = DirectX::XMMatrixRotationY(m_angle);

	view = DirectX::XMMatrixLookAtLH({ 0, 10, -30 }, { 0, 10, 0 }, { 0, 1, 0 });
	proj = DirectX::XMMatrixPerspectiveFovLH(3.141592654f / 4.0f, 800.0f / 600.0f, 1.0f, 1000.0f);

	//“]’u
	world = XMMatrixTranspose(world);
	view = XMMatrixTranspose(view);
	proj = XMMatrixTranspose(proj);

	m_effect->setParam(world, view, proj, lightDir, diffuseColor);
	m_model->draw(m_effect);
}
