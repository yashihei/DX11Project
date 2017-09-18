// -----------------------------------------------
// File : ToonEffect.cpp
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#include "ToonEffect.h"

#include <d3dcompiler.h>
#include <stdexcept>
#include "ShaderRV.h"

namespace {
	struct ConstantBuffer {
		Matrix world;
		Matrix view;
		Matrix proj;
		Vector4 diffuseColor;
		Vector4 ambientColor;
		Vector3 lightDir;
		float padding;
	};
}

inline void compileFromFile(WCHAR * filePath, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob ** blobOut)
{
	DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS;
	ComPtr<ID3DBlob> errorBlob;

#if defined(DEBUG) || defined(_DEBUG)
	flags = D3DCOMPILE_SKIP_OPTIMIZATION;
	flags |= D3DCOMPILE_DEBUG;
#endif

	HRESULT hr = D3DCompileFromFile(filePath, NULL, NULL, entryPoint, shaderModel, flags, 0, blobOut, &errorBlob);

	if (FAILED(hr))
		throw std::runtime_error((char*)errorBlob->GetBufferPointer());
}

ToonEffect::ToonEffect(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext) :
	m_device(device), m_deviceContext(deviceContext)
{
	WCHAR* filePath = L"./Shader/toon.fx";

	//create vs
	ComPtr<ID3DBlob> VSbuffer;
	compileFromFile(filePath, "VS", "vs_5_0", &VSbuffer);
	HRESULT hr = m_device->CreateVertexShader(VSbuffer->GetBufferPointer(), VSbuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(hr))
		throw std::runtime_error("CreateVertexShader() Failed.");
	
	//create ps
	ComPtr<ID3DBlob> PSBuffer;
	compileFromFile(filePath, "PS", "ps_5_0", &PSBuffer);
	hr = m_device->CreatePixelShader(PSBuffer->GetBufferPointer(), PSBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(hr))
		throw std::runtime_error("CreatePixelShader() Failed.");

	//create input layout
	const D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	const UINT numElement = _countof(layout);

	hr = m_device->CreateInputLayout(layout, numElement, VSbuffer->GetBufferPointer(), VSbuffer->GetBufferSize(), &m_layout);
	if (FAILED(hr))
		throw std::runtime_error("CreateInputLayout() Failed.");

	//create constant buf
	D3D11_BUFFER_DESC constantBufferDesc = {};
	constantBufferDesc.ByteWidth = sizeof(ConstantBuffer);
	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	hr = m_device->CreateBuffer(&constantBufferDesc, NULL, &m_constantBuffer);
	if (FAILED(hr))
		throw std::runtime_error("CreateConstantBuffer Failed.");

	m_toonMap = CreateShaderResourceViewFromFile(m_device, L"assets/toon.png");
}

void ToonEffect::setParams(const Matrix& world, const Matrix& view, const Matrix& proj, const Vector3& lightDir, const Vector4& diffuseColor, const Vector4& ambientColor)
{
	D3D11_MAPPED_SUBRESOURCE resource;

	//コンスタントバッファ書き換え
	HRESULT hr = m_deviceContext->Map(m_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	if (SUCCEEDED(hr)) {
		ConstantBuffer* data = reinterpret_cast<ConstantBuffer*>(resource.pData);
		data->world = world;
		data->view = view;
		data->proj = proj;
		data->diffuseColor = diffuseColor;
		data->ambientColor = ambientColor;
		data->lightDir = lightDir;
		data->padding = 0.0f;
		m_deviceContext->Unmap(m_constantBuffer.Get(), 0);
	}
}

void ToonEffect::apply()
{
	//set input layout (TODO:move model class)
	m_deviceContext->IASetInputLayout(m_layout.Get());
	//set texture
	m_deviceContext->PSSetShaderResources(0, 1, m_texture.GetAddressOf());
	m_deviceContext->PSSetShaderResources(1, 1, m_toonMap.GetAddressOf());
	//set shader
	m_deviceContext->VSSetShader(m_vertexShader.Get(), NULL, 0);
	m_deviceContext->PSSetShader(m_pixelShader.Get(), NULL, 0);
	//set constantsbuf
	m_deviceContext->VSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());
	m_deviceContext->PSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());
}
