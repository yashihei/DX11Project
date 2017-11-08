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
#include "DirectXTK/CommonStates.h"

DemoScene::DemoScene(
	ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext,
	InputManagerPtr inputManager, AudioManagerPtr audioManager, CommonStatesPtr states) :
	m_device(device), m_deviceContext(deviceContext),
	m_states(states), m_inputManager(inputManager), m_audioManager(audioManager)
{
	UINT vpCount = 1;
	D3D11_VIEWPORT vp = {};
	m_deviceContext->RSGetViewports(&vpCount, &vp);
	m_camera = std::make_shared<Camera>(Vector3::Zero, Vector3::Zero, Vector3::Up, vp.Width / vp.Height);
	m_camera->pos = Vector3(0, 0, -15);
	m_camera->lookAt = Vector3(0, 0, 0);

	//set states
	m_deviceContext->RSSetState(m_states->CullCounterClockwise());
	m_deviceContext->OMSetBlendState(m_states->NonPremultiplied(), nullptr, 0xFFffFFff);

	m_model = std::make_shared<Model>(m_device, m_deviceContext, m_states);
	//m_model->createFromPmx("assets/alicia/Alicia_solid.pmx");
	m_model->createFromObj("assets/ball/ball3.obj");
}

Scene* DemoScene::update()
{
	m_inputManager->update();
	m_audioManager->update();

	if (m_inputManager->isClicledButton1())
	{
	}

	return this;
}

void DemoScene::draw()
{
	static float y = 0.f;
	y += 0.1f;

	const Matrix world = Matrix::CreateFromYawPitchRoll(y, 0, 0);
	const Matrix view = m_camera->getViewMat();
	const Matrix proj = m_camera->getProjMat();

	m_model->draw(world, view, proj);
}
