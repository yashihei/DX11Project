#pragma once

#include "Fwd.h"
#include "Model.h"
#include "InputManager.h"
#include "Camera.h"
#include "DirectXTK/SimpleMath.h"

using namespace DirectX::SimpleMath;

class Player {
public:
	Player(ModelPtr model, InputManagerPtr inputManager, CameraPtr camera) :
		m_model(model), m_inputManager(inputManager), m_camera(camera),
		m_pos(Vector3::Zero), m_rotation(Vector3::Zero) {
	}
	void update() {
		if (m_inputManager->isPressedLeft())
			m_rotation.y -= 0.03f;
		if (m_inputManager->isPressedRight())
			m_rotation.y += 0.03f;
		if (m_inputManager->isPressedUp())
			m_rotation.x += 0.03f;
		if (m_inputManager->isPressedDown())
			m_rotation.x -= 0.03f;

		Matrix rot = Matrix::CreateFromYawPitchRoll(m_rotation.y, m_rotation.x, m_rotation.z);
		m_pos += rot.Backward() / 3.0f;
	}
	void draw() {
		Matrix scale, rot, trans;
		Matrix world, view, proj;

		scale = Matrix::CreateScale(0.3f);
		rot = Matrix::CreateFromYawPitchRoll(m_rotation.y, m_rotation.x, m_rotation.z);
		trans = Matrix::CreateTranslation(m_pos);

		world = scale * rot * trans;
		view = m_camera->getViewMat();
		proj = m_camera->getProjMat();

		m_model->draw(world.Transpose(), view.Transpose(), proj.Transpose());
	}
private:
	ModelPtr m_model;
	InputManagerPtr m_inputManager;
	CameraPtr m_camera;
	Vector3 m_pos, m_rotation;
};
