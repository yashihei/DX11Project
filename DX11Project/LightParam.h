// -----------------------------------------------
// File : LightParam.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#include "DirectXTK/SimpleMath.h"

using namespace DirectX::SimpleMath;

struct LightParam
{
	LightParam() = default;

	LightParam(const Vector4& diffuse, const Vector4& ambient, const Vector3& direction)
		: diffuse(diffuse),
		  ambient(ambient),
		  direction(direction) {}

	Vector4 diffuse;
	Vector4 ambient;
	Vector3 direction;
};
