// -----------------------------------------------
// File : Particle.cpp
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#include "App.h"
#include "Particle.h"
#include "Sprite.h"
#include "Time.h"
#include "AssetsManager.h"
#include "Easing.h"
#include "Log.h"

namespace sp4rk {

Particle::Particle(App* app): m_app(app), m_time(0)
{
}

void Particle::update()
{
	m_time += m_app->getTime()->deltaTime();
}

NormalParticle::NormalParticle(App* app, const Vector3& pos, const Vector3& vector, const Color& color, const float size, const float lifeTime) :
	Particle(app),
	m_pos(pos), m_vector(vector), m_color(color), m_size(size), m_lifeTime(lifeTime)
{
}

void NormalParticle::update()
{
	Particle::update();

	float deltaTime = m_app->getTime()->deltaTime();
	auto t = Easing::InExp(m_time, m_lifeTime, 1, 0);

	m_pos += (m_vector * t) * deltaTime;
	m_size = t;
	m_color.w = t;

	if (m_time > m_lifeTime)
		kill();
}

void NormalParticle::draw()
{
	auto sprite = m_app->getAssetsManager()->getSprite("particle");
	sprite->draw(m_pos, m_color, m_size);
}

SpawnRing::SpawnRing(App* app, const Vector3& pos, Color color):
	Particle(app),
	m_pos(pos), m_color(color), m_size(0.5f)
{
}

void SpawnRing::update()
{
	Particle::update();

	m_size *= 1.1f;
	m_color.w *= 0.95f;
	if (m_size > 10.f)
		kill();
}

void SpawnRing::draw()
{
	auto sprite = m_app->getAssetsManager()->getSprite("ring");
	sprite->draw(m_pos, m_color, m_size);
}

} // namespace sp4rk
