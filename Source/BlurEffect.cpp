// -----------------------------------------------
// File : BlurEffect.cpp
// Copyright (c) 2018 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#include "BlurEffect.h"
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

	struct BlurConstants {
		int SampleCount;
		int Dummy[3];
		Vector4 Offset[16];
	};
}
#pragma pack(pop)

namespace hks {

inline float GaussianDistribution(const Vector2& pos, float rho)
{
	return exp(-(pos.x * pos.x + pos.y * pos.y) / (2.0f * rho * rho));
}

inline BlurConstants CalcBlurParam(int width, int height, Vector2 dir, float deviation)
{
	BlurConstants result;
    result.SampleCount = 15;
    auto tu = 1.0f / float(width);
    auto tv = 1.0f / float(height);

    result.Offset[0].z = GaussianDistribution(Vector2(0.0f, 0.0f), deviation);
    auto total_weight = result.Offset[0].z;

    result.Offset[0].x = 0.0f;
    result.Offset[0].y = 0.0f;

    for(auto i=1; i<8; ++i)
    {
        result.Offset[i].x = dir.x * i * tu;
        result.Offset[i].y = dir.y * i * tv;
        result.Offset[i].z = GaussianDistribution( dir * float(i), deviation );
        total_weight += result.Offset[i].z * 2.0f;
    }

    for(auto i=0; i<8; ++i)
    {
        result.Offset[i].z /= total_weight;
    }

    for(auto i=8; i<15; ++i)
    {
        result.Offset[i].x = -result.Offset[i - 7].x;
        result.Offset[i].y = -result.Offset[i - 7].y;
        result.Offset[i].z =  result.Offset[i - 7].z;
    }

    return result;
}

BlurEffect::BlurEffect(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext) :
	m_device(device), m_deviceContext(deviceContext)
{
	WCHAR* filePath = L"assets/shader/blur.fx";

	// Create vs
	ComPtr<ID3DBlob> VSbuffer;
	CompileFromFile(filePath, "VS", "vs_5_0", &VSbuffer);
	HRESULT hr = m_device->CreateVertexShader(VSbuffer->GetBufferPointer(), VSbuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(hr))
		throw std::runtime_error("CreateVertexShader() Failed.");

	// Create ps
	ComPtr<ID3DBlob> PSBuffer;
	CompileFromFile(filePath, "PS", "ps_5_0", &PSBuffer);
	hr = m_device->CreatePixelShader(PSBuffer->GetBufferPointer(), PSBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(hr))
		throw std::runtime_error("CreatePixelShader() Failed.");

	// Create constant buf
	CreateConstantBuffer(m_device, sizeof(ObjectConstants), &m_constantBufferObject);
	CreateConstantBuffer(m_device, sizeof(BlurConstants), &m_constantBufferBlur);

	const D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
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

void BlurEffect::setParams(const Matrix& world, const Matrix& view, const Matrix& proj)
{
	D3D11_MAPPED_SUBRESOURCE resource;

	// コンスタントバッファ書き換え
	HRESULT hr = m_deviceContext->Map(m_constantBufferObject.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	if (SUCCEEDED(hr)) {
		ObjectConstants* data = reinterpret_cast<ObjectConstants*>(resource.pData);
		data->world = world;
		data->view = view;
		data->proj = proj;
		m_deviceContext->Unmap(m_constantBufferObject.Get(), 0);
	}
}

void BlurEffect::setHorizontalBlur()
{
	// Set Blur Param
	D3D11_VIEWPORT vp;
	UINT numVP = 1;
	m_deviceContext->RSGetViewports(&numVP, &vp);
	auto src = CalcBlurParam(static_cast<int>(vp.Width), static_cast<int>(vp.Height), Vector2(1.0f, 0.0f), 5.f);

	D3D11_MAPPED_SUBRESOURCE resource;
	HRESULT hr = m_deviceContext->Map(m_constantBufferBlur.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	if (SUCCEEDED(hr)) {
		BlurConstants* data = reinterpret_cast<BlurConstants*>(resource.pData);
		data->SampleCount = src.SampleCount;
		memcpy(data->Offset, src.Offset, sizeof(data->Offset));
		m_deviceContext->Unmap(m_constantBufferBlur.Get(), 0);
	}
}

void BlurEffect::setVerticalBlur()
{
	// Set Blur Param
	D3D11_VIEWPORT vp;
	UINT numVP = 1;
	m_deviceContext->RSGetViewports(&numVP, &vp);
	auto src = CalcBlurParam(static_cast<int>(vp.Width), static_cast<int>(vp.Height), Vector2(0.0f, 1.0f), 5.f);

	D3D11_MAPPED_SUBRESOURCE resource;
	HRESULT hr = m_deviceContext->Map(m_constantBufferBlur.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	if (SUCCEEDED(hr)) {
		BlurConstants* data = reinterpret_cast<BlurConstants*>(resource.pData);
		data->SampleCount = src.SampleCount;
		memcpy(data->Offset, src.Offset, sizeof(data->Offset));
		m_deviceContext->Unmap(m_constantBufferBlur.Get(), 0);
	}
}

void BlurEffect::apply()
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
	ID3D11Buffer* buffers[2] = { m_constantBufferObject.Get(), m_constantBufferBlur.Get() };
	m_deviceContext->VSSetConstantBuffers(0, 2, buffers);
	m_deviceContext->PSSetConstantBuffers(0, 2, buffers);
}

}
