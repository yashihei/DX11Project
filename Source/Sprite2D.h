// -----------------------------------------------
// File : Sprite2D.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <DirectXTK/SimpleMath.h>
#include "Fwd.h"
#include "PtrAlias.h"
#include "MathAlias.h"

namespace hks {

using Microsoft::WRL::ComPtr;

class Sprite2D {
public:
	Sprite2D(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, ComPtr<ID3D11ShaderResourceView> texture);
	void draw(const Vector2& pos, float radian = 0.0f, float scale = 1.0f, const Color& color = { 1, 1, 1, 1 });
	void drawAt(const Vector2& pos, float radian = 0.0f, float scale = 1.0f, const Color& color = { 1, 1, 1, 1 });
	void setTexture(ComPtr<ID3D11ShaderResourceView> texture);
private:
	void createVertexBuffer();
	void createIndexBuffer();

	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_deviceContext;
	ComPtr<ID3D11Buffer> m_vertexBuffer;
	ComPtr<ID3D11Buffer> m_indexBuffer;

	Vector2 m_size, m_offset;
	SpriteEffectPtr m_spriteEffect;
};

} // namespace hks
