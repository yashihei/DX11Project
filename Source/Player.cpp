// -----------------------------------------------
// File : Player.cpp
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#include "App.h"
#include "Player.h"
#include "UtilFunc.h"
#include "AssetsManager.h"
#include "InputManager.h"
#include "Model.h"
#include "Random.h"
#include "UtilDX.h"
#include "Camera.h"
#include "Log.h"
#include "Time.h"
#include "Window.h"
#include <imgui/imgui.h>

namespace sp4rk {

Player::Player(App* app, BulletManagerPtr bullets):
	m_app(app), m_bullets(bullets),
	m_pos(Vector3::Zero), m_vec(Vector3::Zero), m_rot(),
	m_bulletCount(0), m_stateCount(0),
	m_life(3), m_state(State::Normal)
{
}

void Player::update()
{
	m_bulletCount += m_app->getTime()->deltaTime();
	m_stateCount += m_app->getTime()->deltaTime();

	auto moveDir = getMoveDir();
	auto fireDir = getFireDir();

	switch (m_state) {
	case State::Born:
		break;
	case State::Normal:
		move(moveDir);
		fire(fireDir);
		break;
	case State::Die:
		if (m_stateCount > 1.5f) {
			m_stateCount = 0;
			m_life -= 1;
			if (m_life > 0) {
				m_state = State::Normal;
			}
		}
		break;
	}
}

void Player::draw()
{
	if (m_state != State::Normal)
		return;

	auto model = m_app->getAssetsManager()->getModel("player");
	model->draw(m_pos, m_rot);
}

void Player::destroy()
{
	if (m_state != State::Normal)
		return;
	m_state = State::Die;
	m_vec = Vector3::Zero;
	m_stateCount = 0;
}

Vector2 Player::getMoveDir() const
{
	auto moveDir = Vector2::Zero;
	auto input = m_app->getInputManager();

	if (input->isConnectedPad()) {
		moveDir.x = input->getLeftThumbX();
		moveDir.y = input->getLeftThumbY();
	} else {
		moveDir.x = static_cast<float>(input->isPressedRight() - input->isPressedLeft());
		moveDir.y = static_cast<float>(input->isPressedUp() - input->isPressedDown());
	}

	return moveDir;
}

Vector2 Player::getFireDir() const
{
	auto fireDir = Vector2::Zero;
	auto input = m_app->getInputManager();

	if (input->isConnectedPad()) {
		fireDir.x = input->getRightThumbX();
		fireDir.y = input->getRightThumbY();
	} else {
		// マウス座標へのレイを飛ばす
		const auto mousePos = input->getMousePos();
		const Viewport vp(0, 0, static_cast<float>(m_app->getWindow()->getWidth()), static_cast<float>(m_app->getWindow()->getHeight()));
		const auto proj = m_app->getCamera()->getProjMat();
		const auto view = m_app->getCamera()->getViewMat();
		auto ray = GetMouseRay(mousePos, vp, proj, view);
		// レイと地面の交点へfireDirを向ける
		float distance = 0;
		ray.Intersects(Plane(Vector3::Zero, Vector3::Up), distance);
		const auto hitPos = ray.position + ray.direction * distance;
		fireDir.x = hitPos.x - m_pos.x;
		fireDir.y = hitPos.z - m_pos.z;
	}

	return fireDir;
}

void Player::move(Vector2& moveDir)
{
	const float deltaTime = m_app->getTime()->deltaTime();
	const float accel = 2.5f;
	const float maxSpeed = 17.0f;

	if (moveDir != Vector2::Zero) {
		moveDir.Normalize();
		moveDir *= accel;
		m_vec.x += moveDir.x;
		m_vec.z += moveDir.y;
	}

	m_vec *= (1.f - deltaTime * 3.f);
	m_vec = DirectX::XMVector3ClampLength(m_vec, 0, maxSpeed);

	m_pos += m_vec * deltaTime;
	m_pos.Clamp(Vector3(-30.f), Vector3(30.f));

	m_rot *= Quaternion::CreateFromAxisAngle(Vector3::Right, m_vec.z * deltaTime);
	m_rot *= Quaternion::CreateFromAxisAngle(Vector3::Forward, m_vec.x * deltaTime);
}

void Player::fire(Vector2& fireDir)
{
	float fireInterval = 0.075f;
	float fireSpeed = 50.f;

	if (fireDir == Vector2::Zero)
		return;

	if (m_bulletCount < fireInterval) {
		return;
	}
	m_bulletCount = 0;

	fireDir.Normalize();
	// 弾を多少ばら撒く
	float rad = DirectX::XMConvertToRadians(5);
	rad = Random(-rad, rad);
	fireDir = Vector2::Transform(fireDir, Matrix::CreateRotationZ(rad));

	auto fireVec = Vector3(fireDir.x, 0, fireDir.y) * fireSpeed;
	auto bullet = std::make_shared<Bullet>(m_app, m_pos + Vector3(fireDir.x, 0, fireDir.y) * 0.5f, fireVec);
	m_bullets->add(bullet);
}

} // namespace sp4rk
