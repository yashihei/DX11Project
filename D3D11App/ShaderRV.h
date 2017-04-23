#pragma once

#include <wrl\client.h>
#include <stdexcept>
#include <memory>

#include "DirectXTex\DirectXTex.h"
#if defined(DEBUG) || defined(_DEBUG)
	#pragma comment(lib, "DirectXTexD.lib")
#else
	#pragma comment(lib, "DirectXTex.lib")
#endif

using Microsoft::WRL::ComPtr;
using namespace DirectX;

class ShaderRV {
public:
	ShaderRV(ComPtr<ID3D11Device> device, const WCHAR* filePath) {
		TexMetadata info;
		ScratchImage image;

		HRESULT hr = LoadFromWICFile(filePath, 0, &info, image);
		if (FAILED(hr))
			throw std::runtime_error("LoadFromWICFile() Failed.");

		hr = CreateShaderResourceView(device.Get(), image.GetImages(), image.GetImageCount(), info, &m_resource);
		if (FAILED(hr))
			throw std::runtime_error("CreateShaderResourceView() Failed.");
	}

	ComPtr<ID3D11ShaderResourceView> getResource() {
		return m_resource;
	}
private:
	ComPtr<ID3D11ShaderResourceView> m_resource;
};

using ShaderRVPtr = std::shared_ptr<ShaderRV>;
