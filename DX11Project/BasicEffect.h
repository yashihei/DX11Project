// -----------------------------------------------
// File : BasicEffect.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <DirectXTK/SimpleMath.h>

using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;

class BasicEffect {
public:
	BasicEffect(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
	void setObjectParams(const Matrix& world, const Matrix& view, const Matrix& proj);
	void setLightParams(const Vector4& diffuse, const Vector4& ambient, const Vector3& lightDir);
	void setMaterialParams(const Vector4& diffuse, const Vector4& ambient, const Vector4& specular, float power);
	void setTexture(ComPtr<ID3D11ShaderResourceView> texture) { m_texture = texture; }
	void apply();
private:
	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_deviceContext;
	ComPtr<ID3D11VertexShader> m_vertexShader;
	ComPtr<ID3D11PixelShader> m_pixelShader;
	ComPtr<ID3D11Buffer> m_constantBufferObject;
	ComPtr<ID3D11Buffer> m_constantBufferLight;
	ComPtr<ID3D11Buffer> m_constantBufferMaterial;
	ComPtr<ID3D11InputLayout> m_layout;
	ComPtr<ID3D11ShaderResourceView> m_texture;
};
