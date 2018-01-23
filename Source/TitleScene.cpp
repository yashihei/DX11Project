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
#include "Random.h"
#include "Time.h"
#include <DirectXTK/SpriteBatch.h>
#include <DirectXTK/SpriteFont.h>
#include <DirectXTK/CommonStates.h>

namespace sp4rk {

inline void emitPatricle(App* app, ParticleManagerPtr particles, int32 num, const Vector3& pos, const Color& color, float size, float speed, float lifeTime)
{
	for (int32 i = 0; i < num; i++) {
		Quaternion rotate = Quaternion::CreateFromYawPitchRoll(Random(DirectX::XM_2PI), Random(DirectX::XM_2PI), Random(DirectX::XM_2PI));
		Vector3 vec(speed, 0, 0);
		vec = Vector3::Transform(vec, rotate);
		auto particle = std::make_shared<NormalParticle>(app, pos, vec, color, size, lifeTime);
		particles->add(particle);
	}
}

TitleScene::TitleScene(App* app) : m_app(app), m_textCount(0), m_colorCount(0)
{
	auto device = m_app->getGraphics()->getDevice();
	auto deviceContext = m_app->getGraphics()->getDeviceContext();
	auto camera = m_app->getCamera();
	auto states = m_app->getStates();
	auto light = m_app->getLight();
	auto assetsManager = m_app->getAssetsManager();

	camera->pos = Vector3(0, 0, -30.0f);
	camera->lookAt = Vector3(0, 0, 0);
	camera->up = Vector3::Up;

	light->direction = Vector3(0.0f, 0.0f, 1.0f);

	assetsManager->loadSprite("assets/texture/circle.png", "particle", device, deviceContext, camera);
	assetsManager->loadFont("assets/orbitron.spritefont", "orbitron", device);
	m_fontCanvas = std::make_shared<DirectX::SpriteBatch>(deviceContext.Get());

	m_particles = std::make_shared<ActorManager<Particle>>();
}

Scene* TitleScene::update()
{
	if (m_app->getInputManager()->isClickedButton()) {
		m_app->getAssetsManager()->allClear();
		return new PlayScene(m_app);
	}

	m_titleText = "SP4RK!";
	m_textCount += m_app->getTime()->deltaTime();
	float limitTime = 0;
	for (auto& c : m_titleText) {
		limitTime += 0.3f;
		if (m_textCount > limitTime)
			continue;
		c = Random(static_cast<int>('A'), static_cast<int>('Z'));
	}

	if (m_textCount > 7.0f)
		m_textCount = 0;

	m_colorCount += m_app->getTime()->deltaTime() / 3.f;
	emitPatricle(m_app, m_particles, 5, Vector3(8.f, 0, 0), DirectX::XMColorHSVToRGB(Vector3(m_colorCount, 1, 1)), 2.5f, 35, 0.5f);

	m_particles->updateAll();

	return this;
}

void TitleScene::draw()
{
	auto deviceContext = m_app->getGraphics()->getDeviceContext();
	auto states = m_app->getStates();
	deviceContext->OMSetDepthStencilState(states->DepthNone(), 0);
	deviceContext->OMSetBlendState(states->Additive(), 0, 0xFfFfFfFf);
	m_particles->drawAll();
	deviceContext->OMSetBlendState(states->NonPremultiplied(), 0, 0xFfFfFfFf);
	deviceContext->OMSetDepthStencilState(states->DepthDefault(), 0);

	auto font = m_app->getAssetsManager()->getFont("orbitron");
	m_fontCanvas->Begin();
	font->DrawString(m_fontCanvas.get(), s2ws(m_titleText).c_str(), Vector2(15, 275), DirectX::Colors::White, 0, Vector2::Zero, 1.0f);
	font->DrawString(m_fontCanvas.get(), L"PUSH TO ST4RT", Vector2(25, 375), DirectX::Colors::White, 0, Vector2::Zero, 0.5f);
	m_fontCanvas->End();
}

} // namespace sp4rk
