// -----------------------------------------------
// File : PlayScene.cpp
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#include "PlayScene.h"

#include "App.h"
#include "Graphics.h"
#include "Model.h"
#include "InputManager.h"
#include "AudioManager.h"
#include "AssetsManager.h"
#include "Camera.h"
#include "Random.h"
#include "ShaderRV.h"
#include "imgui/imgui.h"
#include "MathAlias.h"
#include "LightParam.h"
#include "Time.h"
#include "Sprite2D.h"
#include "TitleScene.h"
#include "Types.h"
#include "FPSManager.h"
#include "RenderTarget.h"
#include <DirectXTK/SimpleMath.h>
#include <DirectXTK/CommonStates.h>
#include <DirectXColors.h>
#include <array>
#undef max

namespace sp4rk {

inline bool IsCollied(const Vector3& pos1, const Vector3& pos2, float r1, float r2)
{
	const Vector3 tmp = pos2 - pos1;
	return tmp.x * tmp.x + tmp.y * tmp.y + tmp.z * tmp.z < (r1 + r2) * (r1 + r2);
}

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

PlayScene::PlayScene(App* app) : m_app(app), m_spawnCount(0), m_pausing(false), m_viewImgui(true), m_muteki(false)
{
	auto camera = m_app->getCamera();
	auto device = m_app->getGraphics()->getDevice();
	auto deviceContext = m_app->getGraphics()->getDeviceContext();
	auto states = m_app->getStates();
	auto light = m_app->getLight();
	auto assetsManager = m_app->getAssetsManager();

	// Set camera param
	camera->pos = Vector3(0, 50, 0);
	camera->lookAt = Vector3(0, 0, 0);
	camera->up = Vector3::Backward;

	light->direction = Vector3(0.0f, -1.0f, 0.0f);

	// Set states
	deviceContext->RSSetState(states->CullCounterClockwise());
	deviceContext->OMSetBlendState(states->NonPremultiplied(), nullptr, 0xFFffFFff);

	// Load obj models
	assetsManager->loadModel("assets/model/player/player_tex.obj", "player", device, deviceContext, camera, light);
	assetsManager->loadModel("assets/model/enemy/s11enemy.obj", "enemy", device, deviceContext, camera, light);
	assetsManager->loadModel("assets/model/enemy/s11enemy2.obj", "enemy2", device, deviceContext, camera, light);
	assetsManager->loadModel("assets/model/tiled/tiled.obj", "tiled", device, deviceContext, camera, light);

	// Load sprites
	assetsManager->loadSprite("assets/texture/circle_flat.png", "particle", device, deviceContext, camera);
	assetsManager->loadSprite("assets/texture/circle_flat.png", "bullet", device, deviceContext, camera);
	assetsManager->loadSprite("assets/texture/ring.png", "ring", device, deviceContext, camera);

	// Load font
	assetsManager->loadFont("assets/orbitron.spritefont", "orbitron", device);

	// Load 2D sprites
	m_cursor = std::make_shared<Sprite2D>(device, deviceContext, CreateShaderResourceViewFromFile(device, "assets/texture/cursor.png"));

	// Create actor
	m_bullets = std::make_shared<ActorManager<Bullet>>();
	m_particles = std::make_shared<ActorManager<Particle>>();
	m_player = std::make_shared<Player>(m_app, m_bullets);
	m_enemies = std::make_shared<ActorManager<Enemy>>();
	m_score = std::make_shared<Score>();
	m_panel = std::make_shared<Panel>(m_app);

	// Create SubTarget
	D3D11_VIEWPORT vp;
	UINT numVP = 1;
	deviceContext->RSGetViewports(&numVP, &vp);
	m_subTarget = std::make_shared<RenderTarget>(device.Get(), static_cast<int>(vp.Width), static_cast<int>(vp.Height), 4);
}

Scene* PlayScene::update()
{
	// ImGui
	const auto playerPos = m_player->getPos();
	ImGui::Text("PlayerPos : %.2f, %.2f, %.2f", playerPos.x, playerPos.y, playerPos.z);
	ImGui::Text("BulletNum : %d", m_bullets->size());
	ImGui::Text("EnemyNum : %d", m_enemies->size());
	ImGui::Text("ParticleNum : %d", m_particles->size());
	ImGui::Text("FPS : %2.1f", m_app->getFpsManager()->getFps());
	ImGui::Checkbox("MUTEKI MODE", &m_muteki);
	ImGuiLog::instance().Draw("Log");

	// Update actors
	m_player->update();
	m_enemies->updateAll();
	m_particles->updateAll();
	m_bullets->updateAll();
	m_score->update();

	// Set panel params
	m_panel->setLife(std::max(0, m_player->getLife()));
	m_panel->setScore(m_score->getViewScore());

	// Game Over
	if (m_player->getLife() <= 0) {
		m_gameOverCount += m_app->getTime()->deltaTime();
		m_panel->showResult();
		if (m_gameOverCount > 3.0f) {
			m_app->getAssetsManager()->allClear();
			return new TitleScene(m_app);
		}
		return this;
	}

	// Spawn enemies
	m_spawnCount += m_app->getTime()->deltaTime();
	if (m_spawnCount > 1.5f) {
		for (int32 i = 0; i < 5; i++) {
			// Decide spawn pos
			const auto spawnPos = Vector3(Random(-30.0f, 30.0f), 0, Random(-30.0f, 30.0f));
			const auto disPos = spawnPos - m_player->getPos();

			// プレイヤーから近すぎるスポーンはやり直す
			if (disPos.Length() < 3.0f) {
				i--;
				continue;
			}

			// Add enemy
			std::shared_ptr<Enemy> enemy = nullptr;
			if (Random() < 0.5f) {
				enemy = std::make_shared<GreenEnemy>(m_app, spawnPos, m_player->getPos());
			} else {
				enemy = std::make_shared<OrangeEnemy>(m_app, spawnPos, m_player);
			}
			m_enemies->add(enemy);

			// Add ring effect
			auto ring = std::make_shared<SpawnRing>(m_app, spawnPos, enemy->getColor(), 0.5f, 10.0f, 0.5f);
			m_particles->add(ring);

			m_spawnCount = 0;
		}
	}

	// Bomb
	if (m_app->getInputManager()->isClickedButton1()) {
		//TODO:ボム実装
	}

	// Bullet vs enemy
	for (auto& bullet : *m_bullets) {
		for (auto& enemy : *m_enemies) {
			if (IsCollied(bullet->getPos(), enemy->getPos(), 0.5f, 1.0f)) {
				m_score->addScore(1000);
				enemy->kill();
				emitPatricle(m_app, m_particles, 50, enemy->getPos(), enemy->getColor(), 1, 35, 0.5f);
			}
		}
	}

	// Enemy vs player
	bool enemiesClear = false;
	for (auto& enemy : *m_enemies) {
		if (m_muteki) continue;
		if (IsCollied(m_player->getPos(), enemy->getPos(), 0.8f, 0.8f) && m_player->isAlive()) {
			m_player->destroy();
			emitPatricle(m_app, m_particles, 50, m_player->getPos(), Color(0.8f, 0.2f, 0.2f), 1, 45, 1);
			enemiesClear = true;
			m_spawnCount = 0;
			break;
		}
	}

	if (enemiesClear) {
		for (auto& enemy : *m_enemies) {
			emitPatricle(m_app, m_particles, 100, enemy->getPos(), enemy->getColor(), 1, 45, 1);
		}
		m_enemies->clear();
	}

	// Camera control
	auto camera = m_app->getCamera();
	camera->lookAt = m_player->getPos();
	camera->pos = m_player->getPos() + Vector3(0, 50, 0);
	if (m_app->getInputManager()->isPressedButton2()) {
		camera->lookAt = Vector3::Zero;
		camera->pos = Vector3(0, 20, -50);
	}

	// Slow
	if (m_app->getInputManager()->isPressedButton3()) {
		m_app->getTime()->changeScale(0.3f);
	} else {
		m_app->getTime()->changeScale(1.0f);
	}

	// Pause
	if (m_app->getInputManager()->isClickedButton4()) {
		m_pausing = !m_pausing;
	}

	if (m_pausing) {
		m_app->getTime()->changeScale(0);
		m_panel->showPause();
	} else {
		m_panel->hidePause();
	}

	return this;
}

void PlayScene::draw()
{
	auto deviceContext = m_app->getGraphics()->getDeviceContext();
	auto states = m_app->getStates();

	m_player->draw();
	m_enemies->drawAll();

	// Draw sprites
	deviceContext->OMSetDepthStencilState(states->DepthNone(), 0);
	deviceContext->OMSetBlendState(states->Additive(), 0, 0xFfFfFfFf);
	m_particles->drawAll();
	m_bullets->drawAll();
	deviceContext->OMSetBlendState(states->NonPremultiplied(), 0, 0xFfFfFfFf);
	deviceContext->OMSetDepthStencilState(states->DepthDefault(), 0);

	// Draw tile
	deviceContext->RSSetState(states->Wireframe());
	m_app->getAssetsManager()->getModel("tiled")->draw(Vector3::Zero, Vector3::Zero, Vector3::One * 3);
	deviceContext->RSSetState(states->CullCounterClockwise());

	m_panel->draw();
	// SpriteBatchの呼び出しでAlphaBlend, DepthNone, CullCounterClockwise, LinearClampが適用されるので無効化
	deviceContext->OMSetBlendState(states->NonPremultiplied(), 0, 0xFfFfFfFf);
	deviceContext->OMSetDepthStencilState(states->DepthDefault(), 0);

	// Draw cursor
	if (!m_app->getInputManager()->isConnectedPad())
		m_cursor->drawAt(m_app->getInputManager()->getMousePos(), 0, 0.5f, Color(DirectX::Colors::LightGreen));

	drawBlur();

	// Toggle DebugPanel
	if (m_app->getInputManager()->isClickedDebugButton()) {
		m_viewImgui = !m_viewImgui;
	}

	if (m_viewImgui) {
		ImGui::Render();
	}
}

void PlayScene::drawBlur()
{
	auto device = m_app->getGraphics()->getDevice();
	auto deviceContext = m_app->getGraphics()->getDeviceContext();
	auto states = m_app->getStates();

	ID3D11RenderTargetView* backBufferRTV;
	ID3D11DepthStencilView* backBufferDSV;

	// Escape BackBuffer
	deviceContext->OMGetRenderTargets(1, &backBufferRTV, &backBufferDSV);

	// Clear SubTarget
	float color[] = { 0.0f, 0.0f, 0.0f, 1 };
	deviceContext->ClearRenderTargetView(m_subTarget->getRenderTargetView(), color);
	deviceContext->ClearDepthStencilView(m_subTarget->getDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Set SubTarget
	std::array<ID3D11RenderTargetView*, 1> rendertargetViews = { m_subTarget->getRenderTargetView() };
	deviceContext->OMSetRenderTargets(1, rendertargetViews.data(), m_subTarget->getDepthStencilView());

	// Draw Models
	m_player->draw();
	m_enemies->drawAll();

	// Draw sprites
	deviceContext->OMSetDepthStencilState(states->DepthNone(), 0);
	deviceContext->OMSetBlendState(states->Additive(), 0, 0xFfFfFfFf);
	m_particles->drawAll();
	m_bullets->drawAll();
	deviceContext->OMSetBlendState(states->NonPremultiplied(), 0, 0xFfFfFfFf);
	deviceContext->OMSetDepthStencilState(states->DepthDefault(), 0);

	// Set BuckBuffer
	deviceContext->OMSetRenderTargets(1, &backBufferRTV, backBufferDSV);
	
	ImGui::Text("BrightnessMap:");
	ImGui::Image(m_subTarget->getShaderResourceView(), {160, 90}, {0, 0}, {1, 1}, {1, 1, 1, 1}, {1, 1, 1, 0.25});
}

} // namespace sp4rk
