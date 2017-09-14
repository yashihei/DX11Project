// -----------------------------------------------
// File : BasicEffect.cpp
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#include "BasicEffect.h"

#include <d3dcompiler.h>
#include <stdexcept>

#pragma pack(push, 1)
namespace {
	struct ObjectConstants {
		Matrix world;
		Matrix view;
		Matrix proj;
	};

	struct LightConstants {
		Vector4 lightDiffuseColor;
		Vector4 lightAmbientColor;
		Vector3 lightDirection;
		float padding;
	};

	struct MaterialConstants {
		Vector4 diffuseColor;
		Vector4 ambientColor;
		Vector4 specularColor;
		Vector4 power;
	};
}
#pragma pack(pop)

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

inline void createConstantBuffer(ComPtr<ID3D11Device> device, unsigned int byteSize, ID3D11Buffer ** constantBuffer)
{
	D3D11_BUFFER_DESC constantBufferDesc = {};
	constantBufferDesc.ByteWidth = byteSize;
	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = device->CreateBuffer(&constantBufferDesc, NULL, constantBuffer);
	if (FAILED(hr))
		throw std::runtime_error("CreateConstantBuffer Failed.");
}

BasicEffect::BasicEffect(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext) :
	m_device(device), m_deviceContext(deviceContext)
{
	WCHAR* filePath = L"./Shader/basic.fx";

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

	//create constant buf
	createConstantBuffer(m_device, sizeof(ObjectConstants), &m_constantBufferObject);
	createConstantBuffer(m_device, sizeof(LightConstants), &m_constantBufferLight);
	createConstantBuffer(m_device, sizeof(MaterialConstants), &m_constantBufferMaterial);

	//--------------------------------------------------
	//create input layout
	const D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	const UINT numElement = _countof(layout);

	hr = m_device->CreateInputLayout(layout, numElement, VSbuffer->GetBufferPointer(), VSbuffer->GetBufferSize(), &m_layout);
	if (FAILED(hr))
		throw std::runtime_error("CreateInputLayout() Failed.");
}

void BasicEffect::setObjectParams(const Matrix& world, const Matrix& view, const Matrix& proj)
{
	D3D11_MAPPED_SUBRESOURCE resource;

	//コンスタントバッファ書き換え
	//TODO: apply時にコンスタントバッファ書き換えるように
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
		data->lightDiffuseColor = diffuse;
		data->lightAmbientColor = ambient;
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
		data->diffuseColor = diffuse;
		data->ambientColor = ambient;
		data->specularColor = specular;
		data->power = Vector4(power, 0, 0, 0);
		m_deviceContext->Unmap(m_constantBufferMaterial.Get(), 0);
	}
}

void BasicEffect::apply()
{
	//set InputLayout (TODO:move model class)
	m_deviceContext->IASetInputLayout(m_layout.Get());
	//set texture
	m_deviceContext->PSSetShaderResources(0, 1, m_texture.GetAddressOf());
	//set shader
	m_deviceContext->VSSetShader(m_vertexShader.Get(), NULL, 0);
	m_deviceContext->PSSetShader(m_pixelShader.Get(), NULL, 0);
	//set constant buffers
	ID3D11Buffer* buffers[3] = { m_constantBufferObject.Get(), m_constantBufferLight.Get(), m_constantBufferMaterial.Get() };
	m_deviceContext->VSSetConstantBuffers(0, 3, buffers);
	m_deviceContext->PSSetConstantBuffers(0, 3, buffers);
}
