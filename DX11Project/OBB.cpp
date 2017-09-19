// -----------------------------------------------
// File : OBB.cpp (Oriented Bounding Box)
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#include "OBB.h"

#include <algorithm>
#undef min
#undef max

bool CompareLength(const OBB& boxA, const OBB& boxB, const Vector3& vecSeparate, const Vector3& vecDistance)
{
	const float fDistance = std::fabs(vecDistance.Dot(vecSeparate));

	const float fShadowA =
		std::fabs(vecSeparate.Dot(boxA.axisX)) * boxA.size.x / 2 +
		std::fabs(vecSeparate.Dot(boxA.axisY)) * boxA.size.y / 2 +
		std::fabs(vecSeparate.Dot(boxA.axisZ)) * boxA.size.z / 2
	;
	const float fShadowB =
		std::fabs(vecSeparate.Dot(boxB.axisX)) * boxB.size.x / 2 +
		std::fabs(vecSeparate.Dot(boxB.axisY)) * boxB.size.y / 2 +
		std::fabs(vecSeparate.Dot(boxB.axisZ)) * boxB.size.z / 2
	;

	if (fDistance > fShadowA + fShadowB) {
		return false;
	}

	return true;
}

bool OBB::intersets(OBB& other)
{
	const Vector3 vecDistance = worldPos - other.worldPos;

	Vector3* obbAAxis[] = { &axisX, &axisY, &axisZ };
	Vector3* obbBAxis[] = { &other.axisX, &other.axisY, &other.axisZ };

	//OBB-Aの3軸を分離軸にしてチェック
	for (auto axis : obbAAxis) {
		const bool sts = CompareLength(*this, other, *axis, vecDistance);
		if (!sts)
			return false;
	}

	//OBB-Bの3軸を分離軸にしてチェック
	for (auto axis : obbBAxis) {
		const bool sts = CompareLength(*this, other, *axis, vecDistance);
		if (!sts)
			return false;
	}

	//外積を分離軸として計算
	for (auto axisA : obbAAxis) {
		for (auto axisB : obbBAxis) {
			const auto crossSeparate = axisX.Cross(*axisB);
			const bool sts = CompareLength(*this, other, crossSeparate, vecDistance);
			if (!sts)
				return false;
		}
	}

	return true;
}

void OBB::buildBoundingBox(const VertexCollection& vertices)
{
	Vector3 maxPos;
	Vector3 minPos = maxPos = vertices[0].position;

	for (const auto& vertex : vertices) {
		//min
		minPos.x = std::min(minPos.x, vertex.position.x);
		minPos.y = std::min(minPos.y, vertex.position.y);
		minPos.z = std::min(minPos.z, vertex.position.z);
		//max
		maxPos.x = std::max(maxPos.x, vertex.position.x);
		maxPos.y = std::max(maxPos.y, vertex.position.y);
		maxPos.z = std::max(maxPos.z, vertex.position.z);
	}

	center = (maxPos + minPos) / 2;
	worldPos = center;
	size = maxPos - minPos;

	axisX = Vector3::UnitX;
	axisY = Vector3::UnitY;
	axisZ = Vector3::UnitZ;
}

void OBB::transform(Matrix& world)
{
	axisX = Vector3(world._11, world._12, world._13);
	axisY = Vector3(world._21, world._22, world._23);
	axisZ = Vector3(world._31, world._32, world._33);

	worldPos = Vector3::Transform(center, world);

	world._41 = worldPos.x;
	world._42 = worldPos.y;
	world._43 = worldPos.z;
}

void OBB::createBoxMesh(ID3D11DeviceContext* context)
{
	shape = DirectX::GeometricPrimitive::CreateBox(context, size, false);
}

void OBB::draw(const Matrix& world, const Matrix& view, const Matrix& proj, const Color& color)
{
	shape->Draw(world, view, proj, color);
}
