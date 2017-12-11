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

namespace sp4rk {

Particle::Particle(App* app): m_app(app), m_time(0)
{
}

void Particle::update()
{
	m_time += m_app->getTime()->deltaTime();
}

NormalParticle::NormalParticle(App* app, const Vector3& pos, const Vector3& vector, const Color& color, const float scale):
	Particle(app),
	m_pos(pos), m_vector(vector), m_color(color), m_scale(scale)
{
}

void NormalParticle::update()
{
	Particle::update();

	m_pos += m_vector;
	m_vector *= 0.90f;
	m_scale *= 0.95f;
	m_color.w *= 0.95f;
	if (m_color.w < 0.05f)
		kill();
}

void NormalParticle::draw()
{
	auto sprite = m_app->getAssetsManager()->getSprite("particle");
	sprite->draw(m_pos, m_color, m_scale);
}

SpawnRing::SpawnRing(App* app, const Vector3& pos, Color color):
	Particle(app),
	m_pos(pos), m_color(color), m_scale(0.5f)
{
}

void SpawnRing::update()
{
	Particle::update();

	m_scale *= 1.1f;
	m_color.w *= 0.95f;
	if (m_scale > 10.f)
		kill();
}

void SpawnRing::draw()
{
	auto sprite = m_app->getAssetsManager()->getSprite("ring");
	sprite->draw(m_pos, m_color, m_scale);
}

} // namespace sp4rk
