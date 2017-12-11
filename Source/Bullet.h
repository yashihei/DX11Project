// -----------------------------------------------
// File : Bullet.h
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

class Bullet : public Actor {
public:
	Bullet(App* app, const Vector3& spawnPos, const Vector3 vec);
	void update() override;
	void draw() override;
	Vector3 getPos() const { return m_pos; }
private:
	App* m_app;
	Vector3 m_pos, m_vec;
	float m_lifeCount;
};

using BulletManagerPtr = std::shared_ptr<ActorManager<Bullet>>;

} // namespace sp4rk
