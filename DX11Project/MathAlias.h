// -----------------------------------------------
// File : MathAlias.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

namespace DirectX {
	namespace SimpleMath {
		struct Vector2;
		struct Vector3;
		struct Vector4;
		struct Matrix;
		struct Quaternion;
		struct Plane;
		class Ray;
		struct Rectangle;
		struct Color;
	}
}

// alias simplemath
using Vector2 = DirectX::SimpleMath::Vector2;
using Vector3 = DirectX::SimpleMath::Vector3;
using Vector4 = DirectX::SimpleMath::Vector4;
using Matrix = DirectX::SimpleMath::Matrix;
using Quaternion = DirectX::SimpleMath::Quaternion;
using Plane = DirectX::SimpleMath::Plane;
using Ray = DirectX::SimpleMath::Ray;
using Rect = DirectX::SimpleMath::Rectangle;
using Color = DirectX::SimpleMath::Color;
