// -----------------------------------------------
// File : Graphics.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#include <dxgi.h>
#include <d3d11.h>
#include <wrl/client.h>
#include "PtrAlias.h"

namespace hks {

using Microsoft::WRL::ComPtr;

class Graphics {
public:
	Graphics(HWND hWnd, int screenWidth, int screenHeight, bool fullScreen, bool enableAA);
	void beginScene();
	void endScene();
	ComPtr<ID3D11Device> getDevice() const { return m_device; }
	ComPtr<ID3D11DeviceContext> getDeviceContext() const { return m_deviceContext; }
private:
	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_deviceContext;
	ComPtr<IDXGISwapChain> m_swapChain;
	RenderTargetPtr m_backbuffer;
};

} // namespace hks
