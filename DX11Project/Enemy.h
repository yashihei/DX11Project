﻿// -----------------------------------------------
// File : Enemy.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#include "Actor.h"
#include "Model.h"
#include "Random.h"

class Enemy : public Actor
{
public:
	Enemy(ModelPtr model, Vector3 spawnPos, Vector3 target) :
		m_model(model),
		m_pos(spawnPos), m_rot(Vector3::Zero)
	{
		const float rad = std::atan2f(target.z - m_pos.z, target.x - m_pos.x);
		m_vec = Vector3(std::cos(rad), 0, std::sin(rad)) * 0.1f;
	}

	void update() override
	{
		m_pos += m_vec;
		m_pos.x = Clamp(m_pos.x, -20.f, 20.f);
		m_pos.z = Clamp(m_pos.z, -20.f, 20.f);

		//change vec
		//TODO:timerで制御する
		if (Random() > 0.99f) m_vec.x *= -1;
		if (Random() > 0.99f) m_vec.z *= -1;

		m_rot += Vector3(0, 0.1f, 0.1f);
	}

	void draw() override
	{
		m_model->draw(m_pos, m_rot);
	}

	Vector3 getPos() const { return m_pos; }
private:
	ModelPtr m_model;
	Vector3 m_pos, m_rot, m_vec;
};

using EnemyManagerPtr = std::shared_ptr<ActorManager<Enemy>>;
