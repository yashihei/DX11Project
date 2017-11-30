// -----------------------------------------------
// File : Particle.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#include "Fwd.h"
#include "Sprite.h"
#include "Actor.h"
#include "App.h"
#include "AssetsManager.h"

class Particle : public Actor {
public:
	Particle(App* app, const Vector3& pos, const Vector3& vector, const Color& color, const float scale, bool gravity = false) :
		m_app(app), m_pos(pos), m_vector(vector), m_color(color), m_scale(scale), m_gravity(gravity) {}

	void update() override
	{
		m_pos += m_vector;
		m_vector *= 0.90f;
		m_scale *= 0.95f;
		m_color.w *= 0.95f;
		if (m_color.w < 0.05f)
			kill();

		if (m_gravity)
			m_vector += Vector3(0, -0.1f, 0);
	}

	void draw() override
	{
		auto sprite = m_app->getAssetsManager()->getSprite("particle");
		sprite->draw(m_pos, m_color, m_scale);
	}
private:
	App* m_app;
	Vector3 m_pos, m_vector;
	Color m_color;
	float m_scale;
	bool m_gravity;
};

using ParticlePtr = std::shared_ptr<Particle>;
using ParticleManagerPtr = std::shared_ptr<ActorManager<Particle>>;
