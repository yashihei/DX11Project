#pragma once

#include "DirectXTK/SimpleMath.h"
using namespace DirectX::SimpleMath;

class Camera
{
public:
	Camera(const Vector3& _pos, const Vector3& _lookAt, float aspect) :
		pos(_pos), lookAt(_lookAt) {
		m_proj = DirectX::XMMatrixPerspectiveFovLH(3.141592f / 4.0f, aspect, 0.1f, 1000.0f);
	}
	Matrix getViewMat() const {
		return DirectX::XMMatrixLookAtLH(pos, lookAt, { 0, 1, 0 });
	}
	Matrix getProjMat() const {
		return m_proj;
	}
public:
	Vector3 pos, lookAt;
private:
	Matrix m_proj;
};
