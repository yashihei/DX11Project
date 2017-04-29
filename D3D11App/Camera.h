#pragma once

#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

class Camera
{
public:
	Camera(const Vector3& pos, const Vector3& lookAt, float aspect) :
		m_pos(pos), m_lookAt(lookAt) {
		m_proj = DirectX::XMMatrixPerspectiveFovLH(3.141592f / 4.0f, aspect, 0.1f, 1000.0f);
	}
	Matrix getViewMat() const {
		return DirectX::XMMatrixLookAtLH(m_pos, m_lookAt, { 0, 1, 0 });
	}
	Matrix getProjMat() const {
		return m_proj;
	}
private:
	Vector3 m_pos, m_lookAt;
	Matrix m_proj;
};
