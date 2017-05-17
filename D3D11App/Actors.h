#pragma once

#include <deque>
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
		if (m_inputManager->isPressedLeft()) {
			m_advance.y -= 0.003f;
			m_advance.z += 0.006f;
		}
		if (m_inputManager->isPressedRight()) {
			m_advance.y += 0.003f;
			m_advance.z -= 0.006f;
		}
		if (m_inputManager->isPressedUp())
			m_advance.x += 0.003f;
		if (m_inputManager->isPressedDown())
			m_advance.x -= 0.003f;

		m_advance *= 0.90f;
		m_rotation += m_advance;
		m_rotation.z *= 0.95f;

		m_track.push_front(Matrix::CreateFromYawPitchRoll(m_rotation.y, m_rotation.x, 0));
		if (m_track.size() > 15)
			m_track.pop_back();

		m_pos += m_track.begin()->Backward();

		const Matrix nowRotMat = *m_track.rbegin();
		//tps camera
		m_camera->up = nowRotMat.Up();
		m_camera->pos = m_pos + nowRotMat.Forward() * 20;
		m_camera->lookAt = m_pos + nowRotMat.Backward();
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
	Vector3 m_pos, m_rotation, m_advance;
	std::deque<Matrix> m_track;
};
