// -----------------------------------------------
// File : Bullet.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#include "App.h"
#include "Actor.h"
#include "Sprite.h"
#include "AssetsManager.h"
#include "MathAlias.h"
#include "Time.h"
#include <DirectXTK/SimpleMath.h>
#include <boost/timer.hpp>

namespace sp4rk {

using namespace hks;

class Bullet : public Actor {
public:
	Bullet(App* app, const Vector3& spawnPos, const Vector3 vec) :
		m_app(app), m_pos(spawnPos), m_vec(vec), m_lifeCount(0)
	{
	}

	void update() override
	{
		m_pos += m_vec;

		m_lifeCount += m_app->getTime()->deltaTime();
		if (m_lifeCount > 2.5f) {
			kill();
		}
	}

	void draw() override
	{
		auto sprite = m_app->getAssetsManager()->getSprite("bullet");
		sprite->draw(m_pos, Color(1.0f, 0.7f, 0.0f), 0.75f);
	}

	Vector3 getPos() const { return m_pos; }
private:
	App* m_app;
	Vector3 m_pos, m_vec;
	float m_lifeCount;
};

using BulletManagerPtr = std::shared_ptr<ActorManager<Bullet>>;

} // namespace sp4rk
