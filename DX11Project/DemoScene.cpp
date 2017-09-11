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
#include "ShaderRV.h"
#include "Sprite.h"
#include "Sprite2D.h"
#include "imgui/imgui.h"
#include "Random.h"

DemoScene::DemoScene(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext) :
	m_device(device), m_deviceContext(deviceContext), m_rotation(0)
{
	m_inputManager = std::make_shared<InputManager>();
	m_audioManager = std::make_shared<AudioManager>();
	m_states = std::make_unique<DirectX::CommonStates>(m_device.Get());

	UINT vpCount = 1;
	D3D11_VIEWPORT vp = {};
	m_deviceContext->RSGetViewports(&vpCount, &vp);
	m_camera = std::make_shared<Camera>(Vector3::Zero, Vector3::Zero, Vector3::Up, vp.Width / vp.Height);
	m_camera->pos = Vector3(0, 0, -15);
	m_camera->lookAt = Vector3(0, 0, 0);

	m_alicia = std::make_shared<Model>(m_device, m_deviceContext);
	m_alicia->createFromPmx("assets/alicia/Alicia_solid.pmx");
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

	Matrix world, view, proj;

	world = Matrix::CreateFromYawPitchRoll(rot.y, rot.x, rot.z);
	world *= Matrix::CreateTranslation(0, -15, 0);
	view = m_camera->getViewMat();
	proj = m_camera->getProjMat();

	m_alicia->draw(world, view, proj);
}
