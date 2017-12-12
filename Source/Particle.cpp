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
	m_pos(pos), m_vector(vector), m_color(color), m_size(size), m_startSize(size), m_lifeTime(lifeTime)
{
}

void NormalParticle::update()
{
	Particle::update();

	const float deltaTime = m_app->getTime()->deltaTime();
	const float x = Easing::InExp(m_time, m_lifeTime, 1, 0);

	m_pos += (m_vector * x) * deltaTime;
	m_size = m_startSize * x;
	m_color.w = x;

	if (m_time > m_lifeTime)
		kill();
}

void NormalParticle::draw()
{
	auto sprite = m_app->getAssetsManager()->getSprite("particle");
	sprite->draw(m_pos, m_color, m_size);
}

SpawnRing::SpawnRing(App* app, const Vector3& pos, const Color& color, float lifeTime) :
	Particle(app),
	m_pos(pos), m_color(color), m_size(0), m_lifeTime(lifeTime)
{
}

void SpawnRing::update()
{
	Particle::update();

	m_size = Easing::OutQuart(m_time, m_lifeTime, 0.5f, 10.0f);
	m_color.w = Easing::InQuart(m_time, m_lifeTime, 1, 0);

	if (m_time > m_lifeTime)
		kill();
}

void SpawnRing::draw()
{
	auto sprite = m_app->getAssetsManager()->getSprite("ring");
	sprite->draw(m_pos, m_color, m_size);
}

} // namespace sp4rk
