// -----------------------------------------------
// File : Bullet.cpp
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#include "App.h"
#include "Bullet.h"
#include "Sprite.h"
#include "AssetsManager.h"
#include "Time.h"

namespace sp4rk {

Bullet::Bullet(App* app, const Vector3& spawnPos, const Vector3 vec):
	m_app(app), m_pos(spawnPos), m_vec(vec), m_lifeCount(0)
{
}

void Bullet::update()
{
	const auto deltaTime = m_app->getTime()->deltaTime();

	m_pos += m_vec * deltaTime;

	m_lifeCount += deltaTime;
	if (m_lifeCount > 2.5f) {
		kill();
	}
}

void Bullet::draw()
{
	auto sprite = m_app->getAssetsManager()->getSprite("bullet");
	sprite->draw(m_pos, Color(1.0f, 0.7f, 0.0f), 0.75f);
}

} // namespace sp4rk
