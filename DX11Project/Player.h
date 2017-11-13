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
	Player(InputManagerPtr inputManager, ModelPtr model, CameraPtr camera) :
		m_inputManager(inputManager),
		m_model(model), m_camera(camera),
		m_pos(Vector3::Zero), m_rot(Vector3::Zero), m_vec(Vector3::Zero)
	{}

	void update() override
	{
	}

	void draw() override
	{
		const Matrix world = Matrix::CreateFromYawPitchRoll(m_rot.y, m_rot.x, m_rot.z);
		const Matrix view = m_camera->getViewMat();
		const Matrix proj = m_camera->getProjMat();

		m_model->draw(world, view, proj);
	}
private:
	InputManagerPtr m_inputManager;
	ModelPtr m_model;
	CameraPtr m_camera;
	Vector3 m_pos, m_rot, m_vec;
};

using PlayerPtr = std::shared_ptr<Player>;
