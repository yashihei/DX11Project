﻿// -----------------------------------------------
// File : BlurEffect.h
// Copyright (c) 2018 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <DirectXTK/SimpleMath.h>
#include "MathAlias.h"

namespace hks {

using Microsoft::WRL::ComPtr;

class BlurEffect {
public:
	BlurEffect(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
	void setParams(const Matrix& world, const Matrix& view, const Matrix& proj);
	void setTexture(ComPtr<ID3D11ShaderResourceView> texture) { m_texture = texture; }
	void setHorizontalBlur();
	void setVerticalBlur();
	void apply();
private:
	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_deviceContext;
	ComPtr<ID3D11VertexShader> m_vertexShader;
	ComPtr<ID3D11PixelShader> m_pixelShader;
	ComPtr<ID3D11Buffer> m_constantBufferObject;
	ComPtr<ID3D11Buffer> m_constantBufferBlur;
	ComPtr<ID3D11InputLayout> m_layout;
	ComPtr<ID3D11SamplerState> m_samplerState;
	ComPtr<ID3D11ShaderResourceView> m_texture;
};

}