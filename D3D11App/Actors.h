#pragma once

#include <deque>
#include "Fwd.h"
#include "Model.h"
#include "InputManager.h"
#include "Camera.h"
#include "Actor.h"
#include "DirectXTK/SimpleMath.h"

using namespace DirectX::SimpleMath;

class Player {
public:
	Player(ModelPtr model, InputManagerPtr inputManager, CameraPtr camera) :
		m_model(model), m_inputManager(inputManager), m_camera(camera),
		m_pos(Vector3::Zero), m_rotation(Vector3::Zero) {
	}
	void update() {
		//FIXME:クォータニオンか何かで行き先示して補間しろ
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

		//move
		m_pos += m_track.begin()->Backward();

		//tps camera
		const Matrix nowRotMat = *m_track.rbegin();
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
using PlayerPtr = std::shared_ptr<Player>;

struct Collider {
	Vector3 center;
	float radius;
};

class DebugPlayer {
public:
	DebugPlayer(ModelPtr model, ModelPtr sphere, CameraPtr camera, const std::string id) :
		m_model(model), m_sphere(sphere), m_camera(camera),
		m_pos(Vector3::Zero), m_rotation(Vector3::Zero),
		m_id(id)
	{
		m_model->getBoudingSphere(&m_col.center, &m_col.radius);
	}
	void update() {
	}
	void draw() {
		Matrix scale, rot, trans;

		scale = Matrix::CreateScale(1.0f);
		rot = Matrix::CreateFromYawPitchRoll(m_rotation.y, m_rotation.x, m_rotation.z);
		trans = Matrix::CreateTranslation(m_pos);

		Matrix world = scale * rot * trans;

		m_model->draw(world.Transpose(), m_camera->getViewMat().Transpose(), m_camera->getProjMat().Transpose());

		scale = Matrix::CreateScale(m_col.radius*2);
		rot = Matrix::Identity;
		trans = Matrix::CreateTranslation(m_col.center + m_pos);
		world = scale * rot * trans;

		if (m_viewCollider) {
			m_sphere->draw(world.Transpose(), m_camera->getViewMat().Transpose(), m_camera->getProjMat().Transpose());
		}

		//draw ui
		//--------------------------------------------------
		ImGui::Begin(m_id.c_str());
		if (!ImGui::CollapsingHeader("Transform")) {
			//ImGui::SliderFloat("PosX", &m_pos.x, -100, 100);
			//ImGui::SliderFloat("PosY", &m_pos.y, -100, 100);
			//ImGui::SliderFloat("PosZ", &m_pos.z, -100, 100);

			float vPos[3] = { m_pos.x, m_pos.y, m_pos.z };
			ImGui::SliderFloat3("Position", vPos, -50, 50, "%.3f");
			m_pos = Vector3(vPos);
			float vRot[3] = { m_rotation.x, m_rotation.y, m_rotation.z };
			ImGui::SliderFloat3("Rotation", vRot, 0, DirectX::XM_2PI, "%.3f");
			m_rotation = Vector3(vRot);
		}
		if (!ImGui::CollapsingHeader("Bounding Sphere")) {
			ImGui::Text("LocalPos(%f, %f, %f)", m_col.center.x, m_col.center.y, m_col.center.z);
			Vector3 tPos = m_col.center + m_pos;
			ImGui::Text("WorldPos(%f, %f, %f)", tPos.x, tPos.y, tPos.z);
			ImGui::Text("Radius(%f)", m_col.radius);
		}
		ImGui::Separator();
		ImGui::Checkbox("View Collider", &m_viewCollider);
		ImGui::End();
	}
	float getRadius() {
		return m_col.radius;
	}
	Vector3 getPos() {
		return m_pos + m_col.center;
	}
private:
	ModelPtr m_model, m_sphere;
	CameraPtr m_camera;
	Vector3 m_pos, m_rotation;
	Collider m_col;
	std::string m_id;
	bool m_viewCollider;
};
