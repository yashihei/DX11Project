// -----------------------------------------------
// File : Player.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#include "App.h"
#include "Actor.h"
#include "UtilFunc.h"
#include "Bullet.h"
#include "AssetsManager.h"
#include "InputManager.h"
#include "Model.h"
#include "Random.h"
#include "UtilDX.h"
#include "Camera.h"
#include "Log.h"
#include "Time.h"
#include "Window.h"
#include "MathAlias.h"
#include <DirectXTK/SimpleMath.h>
#include <imgui/imgui.h>

namespace sp4rk {

using namespace hks;

class Player : public Actor {
public:
	Player(App* app, BulletManagerPtr bullets) :
		m_app(app), m_bullets(bullets),
		m_pos(Vector3::Zero), m_vec(Vector3::Zero), m_rot(),
		m_bulletCount(0), m_stateCount(0),
		m_life(3), m_state(State::Normal)
	{
	}

	void update() override
	{
		m_bulletCount += m_app->getTime()->deltaTime();
		m_stateCount += m_app->getTime()->deltaTime();

		auto moveDir = getMoveDir();
		auto fireDir = getFireDir();

		switch (m_state) {
		case State::Born:
			//create ring * 3
			break;
		case State::Normal:
			move(moveDir);
			fire(fireDir);
			break;
		case State::Die:
			if (m_stateCount > 1.5f) {
				m_stateCount = 0;
				m_state = State::Normal;
			}
			break;
		default: ;
		}
	}

	void draw() override
	{
		if (m_state != State::Normal)
			return;

		auto model = m_app->getAssetsManager()->getModel("player");
		model->draw(m_pos, m_rot);
	}

	void destroy()
	{
		if (m_state != State::Normal)
			return;
		m_state = State::Die;
		m_life -= 1;
		m_vec = Vector3::Zero;
		m_stateCount = 0;
	}
	
	bool isAlive() const { return m_state == State::Normal; }
	Vector3 getPos() const { return m_pos; }
private:
	Vector2 getMoveDir() const
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

	Vector2 getFireDir() const
	{
		auto fireDir = Vector2::Zero;
		auto input = m_app->getInputManager();

		if (input->isConnectedPad()) {
			fireDir.x = input->getRightThumbX();
			fireDir.y = input->getRightThumbY();
		} else {
			//マウスレイを飛ばす
			const auto mousePos = Vector2(static_cast<float>(input->getMousePosX()), static_cast<float>(input->getMousePosY()));
			const Viewport vp(0, 0, static_cast<float>(m_app->getWindow()->getWidth()), static_cast<float>(m_app->getWindow()->getHeight()));
			const auto proj = m_app->getCamera()->getProjMat();
			const auto view = m_app->getCamera()->getViewMat();
			auto ray = GetMouseRay(mousePos, vp, proj, view);
			//レイと地面の交点へfireDirを向ける
			float distance = 0;
			ray.Intersects(Plane(Vector3::Zero, Vector3::Up), distance);
			const auto hitPos = ray.position + ray.direction * distance;
			fireDir.x = hitPos.x - m_pos.x;
			fireDir.y = hitPos.z - m_pos.z;
		}

		return fireDir;
	}

	void move(Vector2& moveDir)
	{
		if (moveDir != Vector2::Zero) {
			moveDir.Normalize();
			moveDir *= 0.05f;
			m_vec.x += moveDir.x;
			m_vec.z += moveDir.y;
		}

		m_vec *= 0.85f;
		m_pos += m_vec;
		m_pos.x = Clamp(m_pos.x, -20.f, 20.f);
		m_pos.z = Clamp(m_pos.z, -20.f, 20.f);

		m_rot *= Quaternion::CreateFromAxisAngle(Vector3::Right, m_vec.z);
		m_rot *= Quaternion::CreateFromAxisAngle(Vector3::Forward, m_vec.x);
	}

	void fire(Vector2& fireDir)
	{
		float fireInterval = 0.075f;
		float fireSpeed = 0.8f;

		if (fireDir == Vector2::Zero)
			return;

		if (m_bulletCount < fireInterval) {
			return;
		}
		m_bulletCount = 0;

		fireDir.Normalize();
		// scatter bullet
		float rad = DirectX::XMConvertToRadians(5);
		rad = Random(-rad, rad);
		fireDir = Vector2::Transform(fireDir, Matrix::CreateRotationZ(rad));

		auto fireVec = Vector3(fireDir.x, 0, fireDir.y) * fireSpeed;
		auto bullet = std::make_shared<Bullet>(m_app, m_pos, fireVec);
		m_bullets->add(bullet);
	}

	App* m_app;
	BulletManagerPtr m_bullets;
	Vector3 m_pos, m_vec;
	Quaternion m_rot;
	float m_count, m_bulletCount, m_stateCount;
	int m_life;

	enum class State {
		Born, Normal, Die
	} m_state;
};

using PlayerPtr = std::shared_ptr<Player>;

} // namespace sp4rk
