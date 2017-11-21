// -----------------------------------------------
// File : UtilDX.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <stdexcept>
#include <DirectXTK/SimpleMath.h>

using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;

inline Color GetColorFromHSV(float h_, float s_, float v_, float alpha = 1.0f)
{
	const float h = h_ - floor(h_);
	const float s = s_;
	const float v = v_;
	const float hueF = h_ * 6.0f;
	const int hueI = static_cast<int>(hueF);
	const float fr = hueF - hueI;
	const float m = v_ * (1.0f - s_);
	const float n = v_ * (1.0f - s_ * fr);
	const float p = v_ * (1.0f - s_ * (1.0f - fr));

	Color color;
	color.w = alpha;

	switch (hueI) {
	case 0: color.x = v; color.y = p; color.z = m; break;
	case 1: color.x = n; color.y = v; color.z = m; break;
	case 2: color.x = m; color.y = v; color.z = p; break;
	case 3: color.x = m; color.y = n; color.z = v; break;
	case 4: color.x = p; color.y = m; color.z = v; break;
	default: color.x = v; color.y = m; color.z = n; break;
	}

	return color;
}

inline void CompileFromFile(WCHAR* filePath, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** blobOut)
{
	DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;

#if defined(DEBUG) || defined(_DEBUG)
	flags = D3DCOMPILE_SKIP_OPTIMIZATION;
	flags |= D3DCOMPILE_DEBUG;
#endif

	HRESULT hr = D3DCompileFromFile(filePath, NULL, NULL, entryPoint, shaderModel, flags, 0, blobOut, &errorBlob);

	if (FAILED(hr))
		throw std::runtime_error((char*)errorBlob->GetBufferPointer());
}

inline void CreateConstantBuffer(ComPtr<ID3D11Device> device, unsigned int byteSize, ID3D11Buffer** constantBuffer)
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
