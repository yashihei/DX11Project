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
	m_alicia->createFromPmx("assets/alicia/Alicia_blade.pmx");

	m_sphere = std::make_shared<Model>(m_device, m_deviceContext, m_states);
	m_sphere->createFromPmx("assets/sphere/sphere.pmx");

	//create obb
	m_obbA.buildBoundingBox(m_alicia->getVertices());
	m_obbB.buildBoundingBox(m_alicia->getVertices());
	m_obbA.createBoxMesh(m_deviceContext.Get());
	m_obbB.createBoxMesh(m_deviceContext.Get());
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

	Color color(1, 1, 1, 0.5f);
	if (m_obbA.intersets(m_obbB))
	{
		color = Color(1, 0.5, 0.5, 0.5);
	}

	Matrix world = Matrix::CreateFromYawPitchRoll(rot.y, rot.x, rot.z) * Matrix::CreateTranslation(-5, 0, 0);
	const Matrix view = m_camera->getViewMat();
	const Matrix proj = m_camera->getProjMat();

	m_alicia->draw(world, view, proj);
	m_obbA.transform(world);
	m_obbA.draw(world, view, proj, color);

	static Vector3 rot2 = {};
	ImGui::Begin("config2");
	ImGui::SliderFloat("RotationX", &rot2.x, 0, DirectX::XM_2PI, "%.3f");
	ImGui::SliderFloat("RotationY", &rot2.y, 0, DirectX::XM_2PI, "%.3f");
	ImGui::SliderFloat("RotationZ", &rot2.z, 0, DirectX::XM_2PI, "%.3f");
	ImGui::End();

	Matrix world2 = Matrix::CreateFromYawPitchRoll(rot2.y, rot2.x, rot2.z) * Matrix::CreateTranslation(5, 0, 0);
	m_alicia->draw(world2, view, proj);
	m_obbB.transform(world2);
	m_obbB.draw(world2, view, proj, color);
}
