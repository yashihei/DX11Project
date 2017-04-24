#pragma once

#include <d3d11.h>
#include <wrl\client.h>
#include <memory>
#include "SimpleMath.h"

using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;

class Effect {
public:
	Effect(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
	void set(const Matrix& world, const Matrix& view, const Matrix& proj, ComPtr<ID3D11ShaderResourceView> rv, const Vector3& lightDir, const Vector4& diffuseColor);
private:
	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_deviceContext;
	ComPtr<ID3D11VertexShader> m_vertexShader;
	ComPtr<ID3D11PixelShader> m_pixelShader;
	ComPtr<ID3D11InputLayout> m_layout;
	ComPtr<ID3D11Buffer> m_constantBuffer;
	ComPtr<ID3D11SamplerState> m_samplerState;
};

using EffectPtr = std::shared_ptr<Effect>;
