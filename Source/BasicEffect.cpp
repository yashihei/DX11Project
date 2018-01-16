// -----------------------------------------------
// File : BasicEffect.cpp
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#include "BasicEffect.h"

#include <d3dcompiler.h>
#include <stdexcept>
#include "UtilDX.h"

#pragma pack(push, 1)
namespace {
	struct ObjectConstants {
		Matrix world;
		Matrix view;
		Matrix proj;
	};

	struct LightConstants {
		Vector4 diffuseLight;
		Vector4 ambientLight;
		Vector3 lightDirection;
		float padding;
	};

	struct MaterialConstants {
		Vector4 diffuseMaterial;
		Vector4 ambientMaterial;
		Vector4 specularMaterial;
		Vector4 power;
	};
}
#pragma pack(pop)

namespace hks {

BasicEffect::BasicEffect(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext) :
	m_device(device), m_deviceContext(deviceContext)
{
	WCHAR* filePath = L"assets/shader/basic.fx";

	// Create vs
	ComPtr<ID3DBlob> VSbuffer;
	CompileFromFile(filePath, "VS", "vs_5_0", &VSbuffer);
	HRESULT hr = m_device->CreateVertexShader(VSbuffer->GetBufferPointer(), VSbuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(hr))
		throw std::runtime_error("CreateVertexShader() Failed.");

	// Create ps
	ComPtr<ID3DBlob> PSBuffer;
	CompileFromFile(filePath, "PSHalf", "ps_5_0", &PSBuffer);
	hr = m_device->CreatePixelShader(PSBuffer->GetBufferPointer(), PSBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(hr))
		throw std::runtime_error("CreatePixelShader() Failed.");

	// Create constant buf
	CreateConstantBuffer(m_device, sizeof(ObjectConstants), &m_constantBufferObject);
	CreateConstantBuffer(m_device, sizeof(LightConstants), &m_constantBufferLight);
	CreateConstantBuffer(m_device, sizeof(MaterialConstants), &m_constantBufferMaterial);

	// Create input layout
	const D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	const UINT numElement = _countof(layout);

	hr = m_device->CreateInputLayout(layout, numElement, VSbuffer->GetBufferPointer(), VSbuffer->GetBufferSize(), &m_layout);
	if (FAILED(hr))
		throw std::runtime_error("CreateInputLayout() Failed.");

	// Create sampler
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = samplerDesc.AddressV = samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = samplerDesc.BorderColor[1] = samplerDesc.BorderColor[2] = samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = m_device->CreateSamplerState(&samplerDesc, &m_samplerState);
	if (FAILED(hr))
		throw std::runtime_error("CreateSamplerState() Failed.");
}

void BasicEffect::setObjectParams(const Matrix& world, const Matrix& view, const Matrix& proj)
{
	D3D11_MAPPED_SUBRESOURCE resource;

	// コンスタントバッファ書き換え
	// TODO: apply時にコンスタントバッファ書き換えるように
	HRESULT hr = m_deviceContext->Map(m_constantBufferObject.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	if (SUCCEEDED(hr)) {
		ObjectConstants* data = reinterpret_cast<ObjectConstants*>(resource.pData);
		data->world = world;
		data->view = view;
		data->proj = proj;
		m_deviceContext->Unmap(m_constantBufferObject.Get(), 0);
	}
}

void BasicEffect::setLightParams(const Vector4& diffuse, const Vector4& ambient, const Vector3& lightDir)
{
	D3D11_MAPPED_SUBRESOURCE resource;

	HRESULT hr = m_deviceContext->Map(m_constantBufferLight.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	if (SUCCEEDED(hr)) {
		LightConstants* data = reinterpret_cast<LightConstants*>(resource.pData);
		data->diffuseLight = diffuse;
		data->ambientLight = ambient;
		data->lightDirection = lightDir;
		data->padding = 0.0f;
		m_deviceContext->Unmap(m_constantBufferLight.Get(), 0);
	}
}

void BasicEffect::setMaterialParams(const Vector4& diffuse, const Vector4& ambient, const Vector4& specular, float power)
{
	D3D11_MAPPED_SUBRESOURCE resource;

	HRESULT hr = m_deviceContext->Map(m_constantBufferMaterial.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	if (SUCCEEDED(hr)) {
		MaterialConstants* data = reinterpret_cast<MaterialConstants*>(resource.pData);
		data->diffuseMaterial = diffuse;
		data->ambientMaterial = ambient;
		data->specularMaterial = specular;
		data->power = Vector4(power, 0, 0, 0);
		m_deviceContext->Unmap(m_constantBufferMaterial.Get(), 0);
	}
}

void BasicEffect::apply()
{
	// Set sampler
	m_deviceContext->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());
	// Set InputLayout
	m_deviceContext->IASetInputLayout(m_layout.Get());
	// Set texture
	m_deviceContext->PSSetShaderResources(0, 1, m_texture.GetAddressOf());
	// Set shader
	m_deviceContext->VSSetShader(m_vertexShader.Get(), NULL, 0);
	m_deviceContext->PSSetShader(m_pixelShader.Get(), NULL, 0);
	// Set constant buffers
	ID3D11Buffer* buffers[3] = { m_constantBufferObject.Get(), m_constantBufferLight.Get(), m_constantBufferMaterial.Get() };
	m_deviceContext->VSSetConstantBuffers(0, 3, buffers);
	m_deviceContext->PSSetConstantBuffers(0, 3, buffers);
}

} // namespace hks
