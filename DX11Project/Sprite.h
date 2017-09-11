// -----------------------------------------------
// File : Sprite.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include "DirectXTK/SimpleMath.h"
#include "Fwd.h"

using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;

class Sprite {
public:
	Sprite(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, ComPtr<ID3D11ShaderResourceView> texture, CameraPtr camera);
	void draw(const Vector3& pos, float scale);
	void draw(const Vector3& pos, const Color& color, float scale);
private:
	void createVertexBuffer();
	void createIndexBuffer();

	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_deviceContext;
	ComPtr<ID3D11Buffer> m_vertexBuffer;
	ComPtr<ID3D11Buffer> m_indexBuffer;
	ComPtr<ID3D11ShaderResourceView> m_texture;

	SpriteEffectPtr m_spriteEffect;
	CameraPtr m_camera;
};
