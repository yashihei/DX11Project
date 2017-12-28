// -----------------------------------------------
// File : Player.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#include "Actor.h"
#include "Bullet.h"
#include "MathAlias.h"
#include "Types.h"
#include <d3d11.h>
#include <DirectXTK/SimpleMath.h>

namespace sp4rk {

class App;
using namespace hks;

class Player : public Actor {
public:
	Player(App* app, BulletManagerPtr bullets);
	void update() override;
	void draw() override;
	void destroy();
	bool isAlive() const { return m_state == State::Normal; }
	Vector3 getPos() const { return m_pos; }
private:
	Vector2 getMoveDir() const;
	Vector2 getFireDir() const;
	void move(Vector2& moveDir);
	void fire(Vector2& fireDir);

	App* m_app;
	BulletManagerPtr m_bullets;
	Vector3 m_pos, m_vec;
	Quaternion m_rot;
	float m_bulletCount, m_stateCount;
	int32 m_life;

	enum class State {
		Born, Normal, Die
	} m_state;
};

using PlayerPtr = std::shared_ptr<Player>;

} // namespace sp4rk
