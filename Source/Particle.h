// -----------------------------------------------
// File : Particle.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#include "App.h"
#include "Sprite.h"
#include "Actor.h"
#include "Easing.h"
#include "AssetsManager.h"
#include "MathAlias.h"
#include <DirectXTK/SimpleMath.h>
#include <boost/timer.hpp>

namespace sp4rk {

using namespace hks;

class Particle : public Actor {
public:
	explicit Particle(App* app) : m_app(app) {}
protected:
	App* m_app;
	//TODO:deltatime
};

using ParticleManagerPtr = std::shared_ptr<ActorManager<Particle>>;

class NormalParticle : public Particle {
public:
	NormalParticle(App* app, const Vector3& pos, const Vector3& vector, const Color& color, const float scale) :
		Particle(app),
		m_pos(pos), m_vector(vector), m_color(color), m_scale(scale)
	{}

	void update() override
	{
		m_pos += m_vector;
		m_vector *= 0.90f;
		m_scale *= 0.95f;
		m_color.w *= 0.95f;
		if (m_color.w < 0.05f)
			kill();
	}

	void draw() override
	{
		auto sprite = m_app->getAssetsManager()->getSprite("particle");
		sprite->draw(m_pos, m_color, m_scale);
	}
private:
	Vector3 m_pos, m_vector;
	Color m_color;
	float m_scale;
};

//FIXME
class SpawnRing : public Particle {
public:
	SpawnRing(App* app, const Vector3& pos, Color color) :
		Particle(app),
		m_pos(pos), m_color(color), m_scale(0.5f)
	{}

	void update() override
	{
		m_scale *= 1.1f;
		m_color.w *= 0.95f;
		if (m_scale > 10.f)
			kill();
	}

	void draw() override
	{
		auto sprite = m_app->getAssetsManager()->getSprite("ring");
		sprite->draw(m_pos, m_color, m_scale);
	}
private:
	Vector3 m_pos;
	Color m_color;
	float m_scale;
};

} // namespace sp4rk
