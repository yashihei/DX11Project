// -----------------------------------------------
// File : PlayScene.h
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
#include "Score.h"
#include "Panel.h"

namespace sp4rk {

using namespace hks;
using Microsoft::WRL::ComPtr;
class App;

class PlayScene : public Scene {
public:
	explicit PlayScene(App* app);
	Scene* update() override;
	void draw() override;
private:
	App* m_app;
	//actor
	PlayerPtr m_player;
	EnemyManagerPtr m_enemies;
	ParticleManagerPtr m_particles;
	BulletManagerPtr m_bullets;
	ScorePtr m_score;
	PanelPtr m_panel;
	Sprite2DPtr m_cursor;
	float m_spawnCount, m_gameOverCount;;
	bool m_pausing;
};

} // namespace sp4rk
