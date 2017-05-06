#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <memory>
#include "DirectXTK/SimpleMath.h"

using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;

class BasicEffect {
public:
	BasicEffect(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
	void setParam(const Matrix& world, const Matrix& view, const Matrix& proj, const Vector3& lightDir, const Vector4& diffuseColor);
	void setTexture(ComPtr<ID3D11ShaderResourceView> texture) { m_texture = texture; }
	void apply();
private:
	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_deviceContext;
	ComPtr<ID3D11VertexShader> m_vertexShader;
	ComPtr<ID3D11PixelShader> m_pixelShader;
	ComPtr<ID3D11Buffer> m_constantBuffer;
	ComPtr<ID3D11InputLayout> m_layout;
	ComPtr<ID3D11SamplerState> m_samplerState;
	ComPtr<ID3D11ShaderResourceView> m_texture;

	//Matrix m_world, m_view, m_proj;
	//Vector3 m_lightDir, m_LightDiffuseColor;
};
