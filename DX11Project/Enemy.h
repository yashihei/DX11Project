// -----------------------------------------------
// File : Enemy.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#include "Actor.h"
#include "Model.h"

class Enemy : public Actor
{
public:
	Enemy(ModelPtr model, Vector3 spawnPos) :
		m_model(model),
		m_pos(spawnPos), m_rot(Vector3::Zero)
	{
	}

	void update() override
	{
		const float rad = std::atan2f(0 - m_pos.z, 0 - m_pos.x);
		m_pos += Vector3(std::cos(rad), 0, std::sin(rad)) * 0.1f;
		m_rot += Vector3(0, 0.1f, 0.1f);
	}

	void draw() override
	{
		m_model->draw(m_pos, m_rot);
	}
private:
	ModelPtr m_model;
	Vector3 m_pos, m_rot;
};

using EnemyManagerPtr = std::shared_ptr<ActorManager<Enemy>>;
