// -----------------------------------------------
// File : ToonEffect.cpp
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#include "ToonEffect.h"

#include <d3dcompiler.h>
#include <stdexcept>
#include "ShaderRV.h"
#include "UtilDX.h"

#pragma pack(push, 1)
namespace {
	struct ObjectParams {
		Matrix world;
		Matrix view;
		Matrix proj;
	};

	struct LightParams {
		Vector4 diffuseLight;
		Vector4 ambientLight;
		Vector3 lightDirection;
		float padding;
	};

	struct MaterialParams {
		Vector4 diffuseMaterial;
		Vector4 ambientMaterial;
		Vector4 specularMaterial;
		Vector4 power;
	};
}
#pragma pack(pop)

namespace hks {

ToonEffect::ToonEffect(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext) :
	m_device(device), m_deviceContext(deviceContext)
{
	WCHAR* filePath = L"assets/shader/toon.fx";

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
	CreateConstantBuffer(m_device, sizeof(ObjectParams), &m_objectParams);
	CreateConstantBuffer(m_device, sizeof(LightParams), &m_lightParams);
	CreateConstantBuffer(m_device, sizeof(MaterialParams), &m_materialParams);

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
	samplerDesc.AddressU = samplerDesc.AddressV = samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = samplerDesc.BorderColor[1] = samplerDesc.BorderColor[2] = samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = m_device->CreateSamplerState(&samplerDesc, &m_samplerState);
	if (FAILED(hr))
		throw std::runtime_error("CreateSamplerState() Failed.");

	m_toonMap = CreateShaderResourceViewFromFile(m_device, "assets/texture/toon3.png");
}

void ToonEffect::setObjectParams(const Matrix& world, const Matrix& view, const Matrix& proj)
{
	D3D11_MAPPED_SUBRESOURCE resource;

	HRESULT hr = m_deviceContext->Map(m_objectParams.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	if (SUCCEEDED(hr)) {
		ObjectParams* data = reinterpret_cast<ObjectParams*>(resource.pData);
		data->world = world;
		data->view = view;
		data->proj = proj;
		m_deviceContext->Unmap(m_objectParams.Get(), 0);
	}
}

void ToonEffect::setLightParams(const Vector4& diffuse, const Vector4& ambient, const Vector3& lightDir)
{
	D3D11_MAPPED_SUBRESOURCE resource;

	HRESULT hr = m_deviceContext->Map(m_lightParams.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	if (SUCCEEDED(hr)) {
		LightParams* data = reinterpret_cast<LightParams*>(resource.pData);
		data->diffuseLight = diffuse;
		data->ambientLight = ambient;
		data->lightDirection = lightDir;
		data->padding = 0.0f;
		m_deviceContext->Unmap(m_lightParams.Get(), 0);
	}
}

void ToonEffect::setMaterialParams(const Vector4& diffuse, const Vector4& ambient, const Vector4& specular, float power)
{
	D3D11_MAPPED_SUBRESOURCE resource;

	HRESULT hr = m_deviceContext->Map(m_materialParams.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	if (SUCCEEDED(hr)) {
		MaterialParams* data = reinterpret_cast<MaterialParams*>(resource.pData);
		data->diffuseMaterial = diffuse;
		data->ambientMaterial = ambient;
		data->specularMaterial = specular;
		data->power = Vector4(power, 0, 0, 0);
		m_deviceContext->Unmap(m_materialParams.Get(), 0);
	}
}

void ToonEffect::apply()
{
	// Set sampler
	m_deviceContext->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());
	// Set input layout
	m_deviceContext->IASetInputLayout(m_layout.Get());
	// Set texture
	m_deviceContext->PSSetShaderResources(0, 1, m_texture.GetAddressOf());
	m_deviceContext->PSSetShaderResources(1, 1, m_toonMap.GetAddressOf());
	// Set shader
	m_deviceContext->VSSetShader(m_vertexShader.Get(), NULL, 0);
	m_deviceContext->PSSetShader(m_pixelShader.Get(), NULL, 0);
	// Set constant buffers
	ID3D11Buffer* buffers[3] = { m_objectParams.Get(), m_lightParams.Get(), m_materialParams.Get() };
	m_deviceContext->VSSetConstantBuffers(0, 3, buffers);
	m_deviceContext->PSSetConstantBuffers(0, 3, buffers);
}

} // namespace hks
