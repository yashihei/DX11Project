// -----------------------------------------------
// File : TitleScene.cpp
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#include "TitleScene.h"
#include "App.h"
#include "Graphics.h"
#include "AssetsManager.h"
#include "Model.h"
#include "Camera.h"
#include "LightParam.h"
#include "InputManager.h"
#include "DemoScene.h"
#include "UtilStr.h"
#include <DirectXTK/SpriteBatch.h>
#include <DirectXTK/SpriteFont.h>

TitleScene::TitleScene(App* app) : m_app(app)
{
	auto device = m_app->getGraphics()->getDevice();
	auto deviceContext = m_app->getGraphics()->getDeviceContext();
	auto camera = m_app->getCamera();
	auto states = m_app->getStates();
	auto light = m_app->getLight();
	auto assetsManager = m_app->getAssetsManager();

	camera->pos = Vector3(0, 0, -3.0f);
	camera->lookAt = Vector3(0, 0, 0);
	camera->up = Vector3::Up;

	light->direction = Vector3(0.0f, 0.0f, 1.0f);

	assetsManager->loadModel("assets/model/ball/ball.obj", "ball", device, deviceContext, states, camera, light);
	assetsManager->loadFont("assets/orbitron.spritefont", "orbitron", device);

	m_fontCanvas = std::make_shared<DirectX::SpriteBatch>(deviceContext.Get());
}

Scene* TitleScene::update()
{
	if (m_app->getInputManager()->isClickedButton()) {
		m_app->getAssetsManager()->allClear();
		return new DemoScene(m_app);
	}

	m_titleText = "SP4RK!";
	float limitTime = 0;
	for (auto& c : m_titleText) {
		limitTime += 0.3f;
		if (m_textTimer.elapsed() > limitTime)
			continue;
		c = Random(static_cast<int>('A'), static_cast<int>('Z'));
	}

	if (m_textTimer.elapsed() > 7.0f)
		m_textTimer.restart();

	return this;
}

void TitleScene::draw()
{
	static float rotY = 0.0f;
	auto input = m_app->getInputManager();
	auto vec = input->isConnectedPad() ? Vector3(input->getLeftThumbY(), -input->getLeftThumbX(), 0) : Vector3::Zero;
	rotY += DirectX::XMConvertToRadians(0.25f);
	m_app->getAssetsManager()->getModel("ball")->draw(Vector3(0.75f, 0, 0), Vector3(0, rotY, 0) + vec);

	auto font = m_app->getAssetsManager()->getFont("orbitron");
	m_fontCanvas->Begin();
	font->DrawString(m_fontCanvas.get(), s2ws(m_titleText).c_str(), Vector2(15, 275), DirectX::Colors::White, 0, Vector2::Zero, 1.0f);
	font->DrawString(m_fontCanvas.get(), L"PUSH TO ST4RT", Vector2(25, 375), DirectX::Colors::White, 0, Vector2::Zero, 0.5f);
	m_fontCanvas->End();
}
