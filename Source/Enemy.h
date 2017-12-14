// -----------------------------------------------
// File : Enemy.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#include "Actor.h"
#include "MathAlias.h"
#include <d3d11.h>
#include <DirectXTK/SimpleMath.h>

namespace sp4rk {

class App;
using namespace hks;

class Enemy : public Actor {
public:
	Enemy(App* app, Vector3 spawnPos, Vector3 target);
	void update() override;
	void draw() override;
	Vector3 getPos() const { return m_pos; }
private:
	App* m_app;
	Vector3 m_pos, m_vec;
	Quaternion m_rot;
	float m_count, m_turnCount;
};

using EnemyManagerPtr = std::shared_ptr<ActorManager<Enemy>>;

} // namespace sp4rk
