#pragma once

#include <DirectXMath.h>
using namespace DirectX;

class Camera
{
public:
	Camera();
	void setPos(XMFLOAT3 pos);
	void setRot(XMFLOAT3 rot);
	XMFLOAT3 GetPos();
	XMFLOAT3 getRot();
private:
	XMFLOAT3 pos, rot;
	XMFLOAT4X4 m_viewMat, m_projMat;
};
