// -----------------------------------------------
// File : Enemy.cpp
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#include "App.h"
#include "Enemy.h"
#include "Model.h"
#include "Random.h"
#include "AssetsManager.h"
#include "UtilFunc.h"
#include "Time.h"
#include <DirectXColors.h>

namespace sp4rk {

Enemy::Enemy(App* app, Vector3 spawnPos):
	m_app(app), m_pos(spawnPos), m_color(DirectX::Colors::White), m_rot(), m_count(0)
{
}

void Enemy::update()
{
	const float deltaTime = m_app->getTime()->deltaTime();
	m_count += deltaTime;

	if (m_count < 0.75f) return;

	m_pos += m_vec * deltaTime;
	m_pos.Clamp(Vector3(-30.f), Vector3(30.f));

	m_rot *= Quaternion::CreateFromAxisAngle(Vector3::Right, m_vec.z * deltaTime);
	m_rot *= Quaternion::CreateFromAxisAngle(Vector3::Forward, m_vec.x * deltaTime);
}

GreenEnemy::GreenEnemy(App* app, Vector3 spawnPos, Vector3 target):
	Enemy(app, spawnPos), m_turnCount(0)
{
	const float speed = 5.0f;
	const float rad = std::atan2f(target.z - m_pos.z, target.x - m_pos.x);
	m_vec = Vector3(std::cos(rad), 0, std::sin(rad)) * speed;
	m_color = Color(0.35f, 0.8f, 0.4f);
}

void GreenEnemy::update()
{
	Enemy::update();
	if (m_count < 0.75f) return;

	m_turnCount += m_app->getTime()->deltaTime();

	// ˆê’èŠm—¦‚Å•ûŒü“]Š·
	if (Random() > (2.0 - m_turnCount)) {
		m_vec.x *= -1;
		m_turnCount = 0;
	} else if (Random() > (2.0 - m_turnCount)) {
		m_vec.z *= -1;
		m_turnCount = 0;
	}
}

void GreenEnemy::draw()
{
	auto model = m_app->getAssetsManager()->getModel("enemy");
	model->draw(m_pos, m_rot);
}

OrangeEnemy::OrangeEnemy(App* app, Vector3 spawnPos):
	Enemy(app, spawnPos)
{
}

void OrangeEnemy::update()
{
	Enemy::update();
	if (m_count < 0.75f) return;
}

void OrangeEnemy::draw()
{
}

} // namespace sp4rk
