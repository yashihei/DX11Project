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
#include <DirectXTK/SimpleMath.h>
#include <DirectXTK/CommonStates.h>
#include <DirectXColors.h>

namespace sp4rk {

inline bool IsCollied(const Vector3& pos1, const Vector3& pos2, float r1, float r2)
{
	const Vector3 tmp = pos2 - pos1;
	return tmp.x * tmp.x + tmp.y * tmp.y + tmp.z * tmp.z < (r1 + r2) * (r1 + r2);
}

inline void emitPatricle(App* app, ParticleManagerPtr particles, int num, const Vector3& pos, const Color& color, float size, float speed, float lifeTime)
{
	for (int i = 0; i < num; i++) {
		Quaternion rotate = Quaternion::CreateFromYawPitchRoll(Random(DirectX::XM_2PI), Random(DirectX::XM_2PI), Random(DirectX::XM_2PI));
		Vector3 vec(speed, 0, 0);
		vec = Vector3::Transform(vec, rotate);
		auto particle = std::make_shared<NormalParticle>(app, pos, vec, color, size, lifeTime);
		particles->add(particle);
	}
}

PlayScene::PlayScene(App* app) : m_app(app), m_spawnCount(0)
{
	auto camera = m_app->getCamera();
	auto device = m_app->getGraphics()->getDevice();
	auto deviceContext = m_app->getGraphics()->getDeviceContext();
	auto states = m_app->getStates();
	auto light = m_app->getLight();
	auto assetsManager = m_app->getAssetsManager();

	//set camera param
	camera->pos = Vector3(0, 50, 0);
	camera->lookAt = Vector3(0, 0, 0);
	camera->up = Vector3::Backward;

	light->direction = Vector3(0.0f, -1.0f, 0.0f);

	//set states
	deviceContext->RSSetState(states->CullCounterClockwise());
	deviceContext->OMSetBlendState(states->NonPremultiplied(), nullptr, 0xFFffFFff);

	//load obj models
	assetsManager->loadModel("assets/model/player/player.obj", "player", device, deviceContext, camera, light);
	assetsManager->loadModel("assets/model/enemy/enemy.obj", "enemy", device, deviceContext, camera, light);
	assetsManager->loadModel("assets/model/tiled/tiled.obj", "tiled", device, deviceContext, camera, light);

	//create sprite
	assetsManager->loadSprite("assets/texture/circle_flat.png", "particle", device, deviceContext, camera);
	assetsManager->loadSprite("assets/texture/circle_flat.png", "bullet", device, deviceContext, camera);
	assetsManager->loadSprite("assets/texture/ring.png", "ring", device, deviceContext, camera);

	//create font
	assetsManager->loadFont("assets/orbitron.spritefont", "orbitron", device);

	auto cursorTex = CreateShaderResourceViewFromFile(device, "assets/texture/cursor.png");
	m_cursor = std::make_shared<Sprite2D>(device, deviceContext, cursorTex);

	//create actor
	m_bullets = std::make_shared<ActorManager<Bullet>>();
	m_particles = std::make_shared<ActorManager<Particle>>();
	m_player = std::make_shared<Player>(m_app, m_bullets);
	m_enemies = std::make_shared<ActorManager<Enemy>>();
	m_score = std::make_shared<Score>(m_app);
}

Scene* PlayScene::update()
{
	m_player->update();
	m_enemies->updateAll();
	m_particles->updateAll();
	m_bullets->updateAll();
	m_score->update();

	m_spawnCount += m_app->getTime()->deltaTime();
	//spawn enemy
	if (m_spawnCount > 2.5f) {
		for (int i = 0; i < 5; i++) {
			const auto spawnPos = Vector3(Random(-20.0f, 20.0f), 0, Random(-20.0f, 20.0f));
			auto enemy = std::make_shared<Enemy>(m_app, spawnPos, m_player->getPos());
			m_enemies->add(enemy);
			auto ring = std::make_shared<SpawnRing>(m_app, spawnPos, Color(0.35f, 0.8f, 0.4f), 0.5f);
			m_particles->add(ring);
			m_spawnCount = 0;
		}
	}

	//bullet vs enemy
	for (auto& bullet : *m_bullets) {
		for (auto& enemy : *m_enemies) {
			if (IsCollied(bullet->getPos(), enemy->getPos(), 0.5f, 1.0f)) {
				m_score->addScore(1000);
				enemy->kill();
				emitPatricle(m_app, m_particles, 50, enemy->getPos(), Color(0.35f, 0.8f, 0.4f), 1, 35, 0.5f);
			}
		}
	}

	//enemy vs player
	bool enemiesClear = false;
	for (auto& enemy : *m_enemies) {
		if (IsCollied(m_player->getPos(), enemy->getPos(), 0.8f, 0.8f)) {
			m_player->destroy();
			emitPatricle(m_app, m_particles, 50, m_player->getPos(), Color(0.8f, 0.2f, 0.2f), 1, 45, 1);
			enemiesClear = true;
			m_spawnCount = 0;
			break;
		}
	}

	if (enemiesClear) {
		for (auto& enemy : *m_enemies) {
			emitPatricle(m_app, m_particles, 100, enemy->getPos(), Color(0.35f, 0.8f, 0.4f), 1, 45, 1);
		}
		m_enemies->clear();
	}

	//tps camera
	auto camera = m_app->getCamera();
	camera->lookAt = m_player->getPos();
	camera->pos = m_player->getPos() + Vector3(0, 50, 0);

	if (m_app->getInputManager()->isPressedButton2()) {
		camera->lookAt = Vector3::Zero;
		camera->pos = Vector3(0, 20, -50);
	}

	//slow
	if (m_app->getInputManager()->isPressedButton3()) {
		m_app->getTime()->changeScale(0.3f);
	} else {
		m_app->getTime()->changeScale(1.0f);
	}

	const auto playerPos = m_player->getPos();
	ImGui::Text("PlayerPos : %.2f, %.2f, %.2f", playerPos.x, playerPos.y, playerPos.z);
	ImGui::Text("BulletNum : %d", m_bullets->size());
	ImGui::Text("EnemyNum : %d", m_enemies->size());
	ImGui::Text("ParticleNum : %d", m_particles->size());

	return this;
}

void PlayScene::draw()
{
	auto deviceContext = m_app->getGraphics()->getDeviceContext();
	auto states = m_app->getStates();

	m_player->draw();
	m_enemies->drawAll();

	//draw sprites
	deviceContext->OMSetDepthStencilState(states->DepthNone(), 0);
	deviceContext->OMSetBlendState(states->Additive(), 0, 0xFfFfFfFf);
	m_particles->drawAll();
	m_bullets->drawAll();
	deviceContext->OMSetBlendState(states->NonPremultiplied(), 0, 0xFfFfFfFf);
	deviceContext->OMSetDepthStencilState(states->DepthDefault(), 0);

	//draw tile
	deviceContext->RSSetState(states->Wireframe());
	m_app->getAssetsManager()->getModel("tiled")->draw(Vector3::Zero, Vector3::Zero, Vector3::One * 2);
	deviceContext->RSSetState(states->CullCounterClockwise());

	m_score->draw();
	deviceContext->OMSetBlendState(states->NonPremultiplied(), 0, 0xFfFfFfFf);
	deviceContext->OMSetDepthStencilState(states->DepthDefault(), 0);

	// draw cursor
	if (!m_app->getInputManager()->isConnectedPad())
		m_cursor->draw(m_app->getInputManager()->getMousePos(), 0, 0.7f, Color(DirectX::Colors::LightGreen));
}

} // namespace sp4rk
