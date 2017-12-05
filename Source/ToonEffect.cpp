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

ToonEffect::ToonEffect(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext) :
	m_device(device), m_deviceContext(deviceContext)
{
	WCHAR* filePath = L"./Shader/toon.fx";

	//create vs
	ComPtr<ID3DBlob> VSbuffer;
	CompileFromFile(filePath, "VS", "vs_5_0", &VSbuffer);
	HRESULT hr = m_device->CreateVertexShader(VSbuffer->GetBufferPointer(), VSbuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(hr))
		throw std::runtime_error("CreateVertexShader() Failed.");

	//create ps
	ComPtr<ID3DBlob> PSBuffer;
	CompileFromFile(filePath, "PS", "ps_5_0", &PSBuffer);
	hr = m_device->CreatePixelShader(PSBuffer->GetBufferPointer(), PSBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(hr))
		throw std::runtime_error("CreatePixelShader() Failed.");

	//create constant buf
	CreateConstantBuffer(m_device, sizeof(ObjectConstants), &m_constantBufferObject);
	CreateConstantBuffer(m_device, sizeof(LightConstants), &m_constantBufferLight);
	CreateConstantBuffer(m_device, sizeof(MaterialConstants), &m_constantBufferMaterial);

	//create input layout
	const D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	const UINT numElement = _countof(layout);

	hr = m_device->CreateInputLayout(layout, numElement, VSbuffer->GetBufferPointer(), VSbuffer->GetBufferSize(), &m_layout);
	if (FAILED(hr))
		throw std::runtime_error("CreateInputLayout() Failed.");

	m_toonMap = CreateShaderResourceViewFromFile(m_device, "assets/texture/toon.png");
}

void ToonEffect::setObjectParams(const Matrix& world, const Matrix& view, const Matrix& proj)
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

void ToonEffect::setLightParams(const Vector4& diffuse, const Vector4& ambient, const Vector3& lightDir)
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

void ToonEffect::setMaterialParams(const Vector4& diffuse, const Vector4& ambient, const Vector4& specular, float power)
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
	//set constant buffers
	ID3D11Buffer* buffers[3] = { m_constantBufferObject.Get(), m_constantBufferLight.Get(), m_constantBufferMaterial.Get() };
	m_deviceContext->VSSetConstantBuffers(0, 3, buffers);
	m_deviceContext->PSSetConstantBuffers(0, 3, buffers);
}
