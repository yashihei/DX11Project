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
#include "PlayScene.h"
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

	assetsManager->loadFont("assets/orbitron.spritefont", "orbitron", device);

	m_fontCanvas = std::make_shared<DirectX::SpriteBatch>(deviceContext.Get());
}

Scene* TitleScene::update()
{
	if (m_app->getInputManager()->isClickedButton()) {
		m_app->getAssetsManager()->allClear();
		return new PlayScene(m_app);
	}

	return this;
}

void TitleScene::draw()
{
	auto font = m_app->getAssetsManager()->getFont("orbitron");
	m_fontCanvas->Begin();
	font->DrawString(m_fontCanvas.get(), L"SP4RK", Vector2(15, 275), DirectX::Colors::White, 0, Vector2::Zero, 1.0f);
	font->DrawString(m_fontCanvas.get(), L"PUSH TO ST4RT", Vector2(25, 375), DirectX::Colors::White, 0, Vector2::Zero, 0.5f);
	m_fontCanvas->End();
}
