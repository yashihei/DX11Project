// -----------------------------------------------
// File : ShaderRV.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#include <wrl/client.h>
#include <stdexcept>
#include <memory>
#include <Shlwapi.h>
#include "DirectXTex/DirectXTex.h"
#include "Utility.h"
#include "Log.h"

inline ComPtr<ID3D11ShaderResourceView> CreateShaderResourceViewFromFile(ComPtr<ID3D11Device> device, const WCHAR* filePath) {
	using Microsoft::WRL::ComPtr;
	using namespace DirectX;

	ComPtr<ID3D11ShaderResourceView> resource;
	TexMetadata info;
	ScratchImage image;
	HRESULT hr;

	if (StrCmpW(L".tga", PathFindExtensionW(filePath)) == 0) {
		hr = LoadFromTGAFile(filePath, &info, image);
		if (FAILED(hr))
			throw std::runtime_error(ws2s(filePath) + " LoadFromTGAFile() Failed.");
	}
	else {
		hr = LoadFromWICFile(filePath, 0, &info, image);
		if (FAILED(hr))
			throw std::runtime_error(ws2s(filePath) + " LoadFromWICFile() Failed.");
	}
	Log("Load %1%.\n", ws2s(filePath));

	hr = CreateShaderResourceView(device.Get(), image.GetImages(), image.GetImageCount(), info, &resource);
	if (FAILED(hr))
		throw std::runtime_error("CreateShaderResourceView() Failed.");

	return resource;
}
