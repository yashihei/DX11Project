// -----------------------------------------------
// File : DemoScene.cpp
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#include "DemoScene.h"

#include "Model.h"
#include "InputManager.h"
#include "AudioManager.h"
#include "Camera.h"
#include "imgui/imgui.h"

DemoScene::DemoScene(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext) :
	m_device(device), m_deviceContext(deviceContext), m_rotation(0)
{
	m_inputManager = std::make_shared<InputManager>();
	m_audioManager = std::make_shared<AudioManager>();
	m_states = std::make_shared<DirectX::CommonStates>(m_device.Get());

	UINT vpCount = 1;
	D3D11_VIEWPORT vp = {};
	m_deviceContext->RSGetViewports(&vpCount, &vp);
	m_camera = std::make_shared<Camera>(Vector3::Zero, Vector3::Zero, Vector3::Up, vp.Width / vp.Height);
	m_camera->pos = Vector3(0, 0, -15);
	m_camera->lookAt = Vector3(0, 0, 0);

	//set states
	m_deviceContext->RSSetState(m_states->CullCounterClockwise());
	m_deviceContext->OMSetBlendState(m_states->NonPremultiplied(), nullptr, 0xFFffFFff);

	m_alicia = std::make_shared<Model>(m_device, m_deviceContext, m_states);
	m_alicia->createFromPmx("assets/alicia/Alicia_solid.pmx");

	m_sphere = std::make_shared<Model>(m_device, m_deviceContext, m_states);
	m_sphere->createFromPmx("assets/sphere/sphere.pmx");
}

Scene* DemoScene::update()
{
	m_inputManager->update();
	m_audioManager->update();

	return this;
}

void DemoScene::draw()
{
	static Vector3 rot = {};
	ImGui::Begin("config");
	ImGui::SliderFloat("RotationX", &rot.x, 0, DirectX::XM_2PI, "%.3f");
	ImGui::SliderFloat("RotationY", &rot.y, 0, DirectX::XM_2PI, "%.3f");
	ImGui::SliderFloat("RotationZ", &rot.z, 0, DirectX::XM_2PI, "%.3f");
	ImGui::End();

	const Matrix world = Matrix::CreateFromYawPitchRoll(rot.y, rot.x, rot.z) * Matrix::CreateTranslation(0, -15, 0);
	const Matrix view = m_camera->getViewMat();
	const Matrix proj = m_camera->getProjMat();

	//m_alicia->draw(world, view, proj);
	m_sphere->draw(Matrix::Identity, view, proj);
}
