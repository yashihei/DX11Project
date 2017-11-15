// -----------------------------------------------
// File : Player.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#include "Fwd.h"
#include "Actor.h"
#include "Model.h"
#include "Camera.h"
#include "InputManager.h"

class Player : public Actor
{
public:
	Player(InputManagerPtr inputManager, ModelPtr model) :
		m_inputManager(inputManager),
		m_model(model),
		m_pos(Vector3::Zero), m_rot(Vector3::Zero), m_vec(Vector3::Zero)
	{}

	void update() override
	{
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
