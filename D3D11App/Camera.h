#pragma once

#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

class Camera
{
public:
	Camera() {}
	void setPos(Vector3 pos) { m_pos = pos; }
	void setRot(Vector3 rot) { m_rot = rot; }
private:
	Vector3 m_pos, m_rot;
	Matrix m_viewMat, m_projMat;
};
