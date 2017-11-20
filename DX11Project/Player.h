// -----------------------------------------------
// File : Player.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#include "Fwd.h"
#include "Actor.h"
#include "Model.h"
#include "InputManager.h"
#include "UtilFunc.h"
#include "Shot.h"

class Player : public Actor {
public:
	Player(InputManagerPtr inputManager, ModelPtr model, ShotManagerPtr shots, SpritePtr shotSprite) :
		m_inputManager(inputManager), m_model(model), m_shotSprite(shotSprite), m_shots(shots),
		m_pos(Vector3::Zero), m_rot(Vector3::Zero), m_vec(Vector3::Zero) {}

	void update() override
	{
		auto moveDir = Vector2::Zero;
		auto fireDir = Vector2::Zero;

		if (m_inputManager->isConnectedPad()) {
			moveDir.x = m_inputManager->getLeftThumbX();
			moveDir.y = m_inputManager->getLeftThumbY();
			fireDir.x = m_inputManager->getRightThumbX();
			fireDir.y = m_inputManager->getRightThumbY();
		} else {
			moveDir.x = static_cast<float>(m_inputManager->isPressedRight() - m_inputManager->isPressedLeft());
			moveDir.y = static_cast<float>(m_inputManager->isPressedUp() - m_inputManager->isPressedDown());
			//TODO: FireDir from mousePos
		}

		moveControl(moveDir);
		fire(fireDir);
	}

	void draw() override
	{
		m_model->draw(m_pos, m_rot);
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

		//FIXME: ローカル軸回転だとやばい><
		m_rot.x += m_vec.z * 0.5f;
		m_rot.z -= m_vec.x * 0.5f;
	}

	void fire(Vector2& fireDir)
	{
		if (fireDir == Vector2::Zero)
			return;

		if (m_shotTimer.elapsed() < 0.05f) {
			return;
		} else {
			m_shotTimer.restart();
		}

		fireDir.Normalize();

		auto shot = std::make_shared<Shot>(m_shotSprite, m_pos, Vector3(fireDir.x, 0, fireDir.y));
		m_shots->add(shot);
	}

	InputManagerPtr m_inputManager;
	ModelPtr m_model;
	SpritePtr m_shotSprite;
	ShotManagerPtr m_shots;
	Vector3 m_pos, m_rot, m_vec;
	boost::timer m_shotTimer;
};

using PlayerPtr = std::shared_ptr<Player>;
