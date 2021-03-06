﻿// -----------------------------------------------
// File : LightParam.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#include <d3d11.h>
#include <DirectXTK/SimpleMath.h>
#include "MathAlias.h"

namespace hks {

struct LightParam {
	LightParam() = default;

	LightParam(const Color& diffuse, const Color& ambient, const Vector3& direction) :
		diffuse(diffuse), ambient(ambient), direction(direction) {}

	Color diffuse, ambient;
	Vector3 direction;
};

} // namespace hks
