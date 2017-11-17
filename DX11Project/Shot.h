// -----------------------------------------------
// File : Shot.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#include "Actor.h"
#include "Sprite.h"

class Shot : public Actor
{
public:
	Shot(SpritePtr sprite, const Vector3& spawnPos, const Vector3 vec) :
		m_sprite(sprite),
		m_pos(spawnPos), m_vec(vec)
	{
	}

	void update() override
	{
		m_pos += m_vec;
	}

	void draw() override
	{
		m_sprite->draw(m_pos, 0.5f);
	}
private:
	SpritePtr m_sprite;
	Vector3 m_pos, m_vec;
};

using ShotManagerPtr = std::shared_ptr<ActorManager<Shot>>;
