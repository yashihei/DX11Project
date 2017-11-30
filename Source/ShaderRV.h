// -----------------------------------------------
// File : ShaderRV.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#include <wrl/client.h>
#include <stdexcept>
#include <Shlwapi.h>
#include <DirectXTex/DirectXTex.h>
#include "UtilStr.h"
#include "Log.h"

inline ComPtr<ID3D11ShaderResourceView> CreateShaderResourceViewFromFile(ComPtr<ID3D11Device> device, const std::string& filePath)
{
	using Microsoft::WRL::ComPtr;
	using namespace DirectX;

	ComPtr<ID3D11ShaderResourceView> resource;
	TexMetadata info;
	ScratchImage image;
	HRESULT hr;

	if (StrCmpW(L".tga", PathFindExtensionW(s2ws(filePath).c_str())) == 0) {
		hr = LoadFromTGAFile(s2ws(filePath).c_str(), &info, image);
		if (FAILED(hr))
			throw std::runtime_error(filePath + " LoadFromTGAFile() Failed.");
	} else {
		hr = LoadFromWICFile(s2ws(filePath).c_str(), 0, &info, image);
		if (FAILED(hr))
			throw std::runtime_error(filePath + " LoadFromWICFile() Failed.");
	}
	Log("Load %1%.\n", filePath);

	hr = CreateShaderResourceView(device.Get(), image.GetImages(), image.GetImageCount(), info, &resource);
	if (FAILED(hr))
		throw std::runtime_error("CreateShaderResourceView() Failed.");

	return resource;
}
