// -----------------------------------------------
// File : DemoScene.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <boost/timer.hpp>
#include "DirectXTK/SimpleMath.h"
#include "Scene.h"
#include "Fwd.h"
#include "Player.h"
#include "Enemy.h"
#include "Particle.h"
#include "Shot.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX::SimpleMath;

class DemoScene : public Scene {
public:
	DemoScene(
		ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext,
		InputManagerPtr inputManager, AudioManagerPtr audioManager, CommonStatesPtr states);
	Scene* update() override;
	void draw() override;
private:
	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_deviceContext;
	CommonStatesPtr m_states;
	InputManagerPtr m_inputManager;
	AudioManagerPtr m_audioManager;
	CameraPtr m_camera;
	LightParamPtr m_light;
	ModelPtr m_playerModel, m_enemyModel, m_tiledModel;
	SpritePtr m_particleSprite, m_shotSprite;
	SpriteBatchPtr m_fontCanvas;
	SpriteFontPtr m_font;
	boost::timer m_spawnTimer;
	//actor
	PlayerPtr m_player;
	EnemyManagerPtr m_enemies;
	ParticleManagerPtr m_particles;
	ShotManagerPtr m_shots;
};
