// -----------------------------------------------
// File : DemoScene.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <boost/timer.hpp>
#include "Fwd.h"
#include "PtrAlias.h"
#include "Scene.h"
#include "Player.h"
#include "Enemy.h"
#include "Particle.h"
#include "Bullet.h"

class App;
using Microsoft::WRL::ComPtr;

class DemoScene : public Scene {
public:
	explicit DemoScene(App* app);
	Scene* update() override;
	void draw() override;
private:
	App* m_app;
	//assets
	SpriteBatchPtr m_fontCanvas;
	//actor
	PlayerPtr m_player;
	EnemyManagerPtr m_enemies;
	ParticleManagerPtr m_particles;
	BulletManagerPtr m_bullets;
	boost::timer m_spawnTimer;
};
