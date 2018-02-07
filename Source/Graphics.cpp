// -----------------------------------------------
// File : Graphics.cpp
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#include "Graphics.h"
#include "RenderTarget.h"
#include <stdexcept>
#include <array>

namespace hks {

void CreateDeviceAndSwapChain(
	HWND hWnd,
	int screenWidth,
	int screenHeight,
	bool fullScreen,
	bool enableAA,
	ComPtr<ID3D11Device>& device,
	ComPtr<ID3D11DeviceContext>& deviceContext,
	ComPtr<IDXGISwapChain>& swapChain)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SampleDesc.Count = enableAA ? 4 : 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = fullScreen ? FALSE : TRUE;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	const std::array<D3D_FEATURE_LEVEL, 7> featureLevels = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		createDeviceFlags,
		featureLevels.data(),
		1,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&swapChain,
		&device,
		NULL,
		&deviceContext
	);

	if (FAILED(hr)) {
		throw std::runtime_error("Failed to create DeviceAndSwapChain.");
	}
}

Graphics::Graphics(HWND hWnd, int32 screenWidth, int32 screenHeight, bool fullScreen, bool enableAA)
{
	CreateDeviceAndSwapChain(hWnd, screenWidth, screenHeight, fullScreen, enableAA, m_device, m_deviceContext, m_swapChain);

	// Create backbuffer
	m_backbuffer = std::make_shared<RenderTarget>(m_device.Get(), m_swapChain.Get(), screenWidth, screenHeight, enableAA ? 4 : 1);

	// Set viewport
	D3D11_VIEWPORT vp = {};
	vp.Width = static_cast<float>(screenWidth);
	vp.Height = static_cast<float>(screenHeight);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_deviceContext->RSSetViewports(1, &vp);
}

void Graphics::beginScene()
{
	float color[] = { 0.1f, 0.1f, 0.1f, 1 };
	m_deviceContext->ClearRenderTargetView(m_backbuffer->getRenderTargetView(), color);
	m_deviceContext->ClearDepthStencilView(m_backbuffer->getDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Use backbuffer
	std::array<ID3D11RenderTargetView*, 1> rendertargetViews = { m_backbuffer->getRenderTargetView() };
	m_deviceContext->OMSetRenderTargets(1, rendertargetViews.data(), m_backbuffer->getDepthStencilView());
}

void Graphics::endScene()
{
	m_swapChain->Present(1, 0);
}

} // namespace hks
