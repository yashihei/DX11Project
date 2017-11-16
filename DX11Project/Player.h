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

class Player : public Actor
{
public:
	Player(InputManagerPtr inputManager, ModelPtr model) :
		m_inputManager(inputManager),
		m_model(model),
		m_pos(Vector3::Zero), m_rot(Vector3::Zero), m_vec(Vector3::Zero)
	{
	}

	void update() override
	{
		Vector2 dir = {
			static_cast<float>(m_inputManager->isPressedRight() - m_inputManager->isPressedLeft()),
			static_cast<float>(m_inputManager->isPressedUp() - m_inputManager->isPressedDown())
		};

		if (dir != Vector2::Zero)
		{
			dir.Normalize();
			dir *= 0.05f;
			m_vec.x += dir.x;
			m_vec.z += dir.y;
		}

		//FIXME: 
		m_rot.x += m_vec.z * 0.5f;
		m_rot.z -= m_vec.x * 0.5f;

		m_vec *= 0.85f;
		m_pos += m_vec;

		//reset
		if (m_inputManager->isClicledButton1())
			m_vec = m_pos = m_rot = Vector3::Zero;
	}

	void draw() override
	{
		m_model->draw(m_pos, m_rot);
	}
private:
	InputManagerPtr m_inputManager;
	ModelPtr m_model;
	Vector3 m_pos, m_rot, m_vec;
};

using PlayerPtr = std::shared_ptr<Player>;
