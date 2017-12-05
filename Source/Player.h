// -----------------------------------------------
// File : Player.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#include "Fwd.h"
#include "Actor.h"
#include "UtilFunc.h"
#include "Bullet.h"
#include "App.h"
#include "AssetsManager.h"
#include "InputManager.h"
#include "Model.h"
#include "Random.h"

class Player : public Actor {
public:
	Player(App* app, BulletManagerPtr bullets) :
		m_app(app), m_bullets(bullets),
		m_pos(Vector3::Zero), m_vec(Vector3::Zero), m_rot(), m_state(State::Born) {}

	void update() override
	{
		auto moveDir = Vector2::Zero;
		auto fireDir = Vector2::Zero;

		auto input = m_app->getInputManager();
		if (input->isConnectedPad()) {
			moveDir.x = input->getLeftThumbX();
			moveDir.y = input->getLeftThumbY();
			fireDir.x = input->getRightThumbX();
			fireDir.y = input->getRightThumbY();
		} else {
			moveDir.x = static_cast<float>(input->isPressedRight() - input->isPressedLeft());
			moveDir.y = static_cast<float>(input->isPressedUp() - input->isPressedDown());
			//TODO: FireDir from mousePos
		}

		moveControl(moveDir);
		fire(fireDir);
	}

	void draw() override
	{
		auto model = m_app->getAssetsManager()->getModel("player");
		model->draw(m_pos, m_rot);
	}

	Vector3 getPos() const { return m_pos; }
private:
	void moveControl(Vector2& moveDir)
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
		if (fireDir == Vector2::Zero)
			return;

		if (m_bulletTimer.elapsed() < 0.05f) {
			return;
		}

		m_bulletTimer.restart();

		fireDir.Normalize();

		// scatter bullet
		float rad = DirectX::XMConvertToRadians(5);
		rad = Random(-rad, rad);
		fireDir = Vector2::Transform(fireDir, Matrix::CreateRotationZ(rad));

		auto bullet = std::make_shared<Bullet>(m_app, m_pos, Vector3(fireDir.x, 0, fireDir.y));
		m_bullets->add(bullet);
	}

	App* m_app;
	BulletManagerPtr m_bullets;
	Vector3 m_pos, m_vec;
	Quaternion m_rot;
	boost::timer m_bulletTimer;

	enum class State {
		Born, Normal, Die
	} m_state;
};

using PlayerPtr = std::shared_ptr<Player>;
