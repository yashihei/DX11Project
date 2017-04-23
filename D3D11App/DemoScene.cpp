#include "DemoScene.h"

#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

DemoScene::DemoScene(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext) :
	m_device(device), m_deviceContext(deviceContext)
{
	m_effect = std::make_shared<Effect>(m_device, m_deviceContext);
	m_model = std::make_shared<Model>(m_device, m_deviceContext);
}

Scene* DemoScene::update()
{
	return this;
}

void DemoScene::draw()
{
	Matrix world, view, proj;

	view = DirectX::XMMatrixLookAtLH({ 0, 0, -5 }, { 0, 0, 0 }, { 0, 1, 0 });
	proj = DirectX::XMMatrixPerspectiveFovLH(3.141592654f / 4.0f, 800 / 600, 1.0f, 1000.0f);

	//“]’u
	world = XMMatrixTranspose(world);
	view = XMMatrixTranspose(view);
	proj = XMMatrixTranspose(proj);

	m_effect->updateConstantBuffer(world, view, proj);
	m_effect->set();

	m_model->draw();
}
