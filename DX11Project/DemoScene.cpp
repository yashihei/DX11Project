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
#include "LightParam.h"
#include "Random.h"
#include "ShaderRV.h"
#include "imgui/imgui.h"
#include "DirectXTK/CommonStates.h"
#include "DirectXTK/SpriteBatch.h"
#include "DirectXTK/SpriteFont.h"

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
	m_camera->pos = Vector3(0, 50, 0);
	m_camera->lookAt = Vector3(0, 0, 0);
	m_camera->up = Vector3::Backward;

	m_light = std::make_shared<LightParam>(Color(1.0f, 1.0f, 1.0f, 1.0f), Color(0.3f, 0.3f, 0.3f, 1.0f), Vector3(0.0f, -1.0f, 0.0f));

	//set states
	m_deviceContext->RSSetState(m_states->CullCounterClockwise());
	m_deviceContext->OMSetBlendState(m_states->NonPremultiplied(), nullptr, 0xFFffFFff);

	//load obj models
	m_playerModel = std::make_shared<Model>(m_device, m_deviceContext, m_states, m_camera, m_light);
	m_playerModel->createFromObj("assets/actors/player.obj");
	m_enemyModel = std::make_shared<Model>(m_device, m_deviceContext, m_states, m_camera, m_light);
	m_enemyModel->createFromObj("assets/actors/enemy.obj");
	m_tiledModel = std::make_shared<Model>(m_device, m_deviceContext, m_states, m_camera, m_light);
	m_tiledModel->createFromObj("assets/tiled/tiled.obj");

	auto tex = CreateShaderResourceViewFromFile(m_device, L"assets/circle.png");
	m_particleSprite = std::make_shared<Sprite>(m_device, m_deviceContext, tex, m_camera);

	m_fontCanvas = std::make_shared<DirectX::SpriteBatch>(m_deviceContext.Get());
	m_font = std::make_shared<DirectX::SpriteFont>(m_device.Get(), L"assets/orbitron.spritefont");

	m_player = std::make_shared<Player>(m_inputManager, m_playerModel);
	m_enemies = std::make_shared<ActorManager<Enemy>>();
	m_particles = std::make_shared<ActorManager<Particle>>();
}

Scene* DemoScene::update()
{
	m_player->update();
	m_enemies->update();
	m_particles->update();

	//spawn enemy
	if (m_spawnTimer.elapsed() > 1.0f)
	{
		const auto spawnPos = Vector3(Random(-20.0f, 20.0f), 0, Random(-20.0f, 20.0f));
		auto enemy = std::make_shared<Enemy>(m_enemyModel, spawnPos, m_player->getPos());
		m_enemies->add(enemy);
		m_spawnTimer.restart();
	}

	//bomb
	if (m_inputManager->isClicledButton1() || m_enemies->size() > 50)
	{
		for (auto& enemy : *m_enemies) {
			for (int i = 0; i < 100; i++) {
				Quaternion rotate = Quaternion::CreateFromYawPitchRoll(Random(DirectX::XM_2PI), Random(DirectX::XM_2PI), Random(DirectX::XM_2PI));
				Vector3 vec(0.75f, 0, 0);
				vec = Vector3::Transform(vec, rotate);
				auto particle = std::make_shared<Particle>(m_particleSprite, enemy->getPos(), vec, Color(0.05f, 0.8f, 0.4f), 0.75f);
				m_particles->add(particle);
			}
		}
		m_enemies->clear();
	}

	const auto playerPos = m_player->getPos();
	ImGui::Begin("DebugPanel");
	ImGui::Text("%f, %f, %f", playerPos.x, playerPos.y, playerPos.z);
	ImGui::Text("EnemyNum : %d", m_enemies->size());
	ImGui::End();

	return this;
}

void DemoScene::draw()
{
	m_player->draw();
	m_enemies->draw();

	m_deviceContext->OMSetDepthStencilState(m_states->DepthNone(), 0);
	m_deviceContext->OMSetBlendState(m_states->Additive(), 0, 0xFfFfFfFf);
	m_particles->draw();
	m_deviceContext->OMSetBlendState(m_states->AlphaBlend(), 0, 0xFfFfFfFf);
	m_deviceContext->OMSetDepthStencilState(m_states->DepthDefault(), 0);

	//draw tile
	m_deviceContext->RSSetState(m_states->Wireframe());
	m_tiledModel->draw(Vector3::Zero, Vector3::Zero, Vector3::One*2);
	m_deviceContext->RSSetState(m_states->CullClockwise());

	//draw text
	m_fontCanvas->Begin();
	m_font->DrawString(m_fontCanvas.get(), L"DEMOPLAY", Vector2::Zero, DirectX::Colors::White, 0, Vector2::Zero, 0.5f);
	m_fontCanvas->End();
	m_deviceContext->OMSetDepthStencilState(m_states->DepthDefault(), 0);
}
