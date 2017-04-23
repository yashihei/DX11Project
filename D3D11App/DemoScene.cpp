#include "DemoScene.h"

#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

#include <fstream>

DemoScene::DemoScene(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext) :
	m_device(device), m_deviceContext(deviceContext)
{
	m_effect = std::make_shared<Effect>(m_device, m_deviceContext);
	m_model = std::make_shared<Model>(m_device, m_deviceContext);
	m_shaderRV = std::make_shared<ShaderRV>(m_device, L"./assets/alice.png");

	//pmx load
	std::filebuf fb;
	if (!fb.open("assets/aichan/kizunaai.pmx", std::ios::in | std::ios::binary)) {
		std::runtime_error("Pmx Load Failed.");
	}
	std::istream is(&fb);

	m_pmxData = std::make_shared<pmx::PmxModel>();
	m_pmxData->Read(&is);
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

	m_effect->set(world, view, proj, m_shaderRV->getResource());

	m_model->draw();
}
