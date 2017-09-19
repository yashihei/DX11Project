// -----------------------------------------------
// File : OBB.h (Oriented Bounding Box)
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#include <d3d11.h>
#include <vector>
#include "DirectXTK/SimpleMath.h"
#include "DirectXTK/VertexTypes.h"
#include "DirectXTK/GeometricPrimitive.h"

using VertexCollection = std::vector<DirectX::VertexPositionNormalTexture>;
using namespace DirectX::SimpleMath;

struct OBB
{
	OBB() = default;

	bool intersets(OBB& other);
	void buildBoundingBox(const VertexCollection& vertices);
	void transform(Matrix& world);
	void createBoxMesh(ID3D11DeviceContext* context);
	void draw(const Matrix& world, const Matrix& view, const Matrix& proj, const Color& color);

	Vector3 axisX;
	Vector3 axisY;
	Vector3 axisZ;
	Vector3 size;
	Vector3 center;
	Vector3 worldPos;

	std::unique_ptr<DirectX::GeometricPrimitive> shape;
};
