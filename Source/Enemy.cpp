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

namespace sp4rk {

Enemy::Enemy(App* app, Vector3 spawnPos, Vector3 target):
	m_app(app), m_pos(spawnPos), m_rot()
{
	const float rad = std::atan2f(target.z - m_pos.z, target.x - m_pos.x);
	m_vec = Vector3(std::cos(rad), 0, std::sin(rad)) * 0.1f;
}

void Enemy::update()
{
	m_pos += m_vec;
	m_pos.x = Clamp(m_pos.x, -20.f, 20.f);
	m_pos.z = Clamp(m_pos.z, -20.f, 20.f);

	//change vec
	//TODO:timerで制御する
	if (Random() > 0.99f) m_vec.x *= -1;
	if (Random() > 0.99f) m_vec.z *= -1;

	m_rot *= Quaternion::CreateFromAxisAngle(Vector3::Right, m_vec.z);
	m_rot *= Quaternion::CreateFromAxisAngle(Vector3::Forward, m_vec.x);
}

void Enemy::draw()
{
	auto model = m_app->getAssetsManager()->getModel("enemy");
	model->draw(m_pos, m_rot);
}

} // namespace sp4rk
