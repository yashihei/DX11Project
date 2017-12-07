// -----------------------------------------------
// File : Bullet.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#include "Actor.h"
#include "Sprite.h"
#include <boost/timer.hpp>
#include "App.h"
#include "AssetsManager.h"

class Bullet : public Actor {
public:
	Bullet(App* app, const Vector3& spawnPos, const Vector3 vec) :
		m_app(app), m_pos(spawnPos), m_vec(vec) {}

	void update() override
	{
		m_pos += m_vec;

		if (m_lifeTimer.elapsed() > 2.5f) {
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
	boost::timer m_lifeTimer;
};

using BulletManagerPtr = std::shared_ptr<ActorManager<Bullet>>;
