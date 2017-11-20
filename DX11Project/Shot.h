// -----------------------------------------------
// File : Shot.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#include "Actor.h"
#include "Sprite.h"
#include <boost/timer.hpp>

class Shot : public Actor {
public:
	Shot(SpritePtr sprite, const Vector3& spawnPos, const Vector3 vec) :
		m_sprite(sprite), m_pos(spawnPos), m_vec(vec) {}

	void update() override
	{
		m_pos += m_vec;

		if (m_lifeTimer.elapsed() > 5.0f) {
			kill();
		}
	}

	void draw() override
	{
		m_sprite->draw(m_pos, Color(1.0f, 0.7f, 0.0f), 0.5f);
	}

	Vector3 getPos() const { return m_pos; }
private:
	SpritePtr m_sprite;
	Vector3 m_pos, m_vec;
	boost::timer m_lifeTimer;
};

using ShotManagerPtr = std::shared_ptr<ActorManager<Shot>>;
