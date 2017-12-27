// -----------------------------------------------
// File : RenderTarget.cpp
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#include "RenderTarget.h"
#include <stdexcept>

namespace hks {

void BuildRenderTarget(
	ID3D11Device* device,
	DXGI_FORMAT format,
	int32 width, int32 height,
	int32 sampleCount,
	ComPtr<ID3D11Texture2D>& renderTargetBuffer,
	ComPtr<ID3D11RenderTargetView>& renderTargetView,
	ComPtr<ID3D11ShaderResourceView>& shaderResourceView)
{
	// Create render texture
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Format = format;
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.ArraySize = 1;
	textureDesc.MipLevels = 1;
	textureDesc.SampleDesc.Count = sampleCount;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	HRESULT hr = device->CreateTexture2D(&textureDesc, nullptr, &renderTargetBuffer);

	if (FAILED(hr)) {
		throw std::runtime_error("Failed to create render target texture");
	}

	// Create RTV
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = textureDesc.Format;
	rtvDesc.ViewDimension = sampleCount > 1 ? D3D11_RTV_DIMENSION_TEXTURE2DMS : D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;

	hr = device->CreateRenderTargetView(renderTargetBuffer.Get(), &rtvDesc, &renderTargetView);
	
	if (FAILED(hr)) {
		throw std::runtime_error("Failed to create RTV");
	}

	// Create SRV
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = sampleCount > 1 ? D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = textureDesc.MipLevels;

	hr = device->CreateShaderResourceView(renderTargetBuffer.Get(), &srvDesc, &shaderResourceView);

	if (FAILED(hr)) {
		throw std::runtime_error("Failed to create SRV");
	}
}

void BuildDepthBuffer(
	ID3D11Device* device,
	DXGI_FORMAT format,
	int32 width, int32 height,
	int32 sampleCount,
	ComPtr<ID3D11Texture2D>& depthStencilBuffer,
	ComPtr<ID3D11DepthStencilView>& depthStencilView)
{
	// Create render texture
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Format = format;
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.ArraySize = 1;
	textureDesc.MipLevels = 1;
	textureDesc.SampleDesc.Count = sampleCount;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	HRESULT hr = device->CreateTexture2D(&textureDesc, nullptr, &depthStencilBuffer);

	if (FAILED(hr)) {
		throw std::runtime_error("Failed to create depth stencil buffer");
	}

	// Create the DSV
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = textureDesc.Format;
	dsvDesc.ViewDimension = sampleCount > 1 ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	hr = device->CreateDepthStencilView(depthStencilBuffer.Get(), &dsvDesc, &depthStencilView);
	
	if (FAILED(hr)) {
		throw std::runtime_error("Failed to create DSV");
	}
}

void BuildBackBufferBySwapChain(
	ID3D11Device* device,
	IDXGISwapChain* swapChain,
	ComPtr<ID3D11Texture2D>& renderTargetBuffer,
	ComPtr<ID3D11RenderTargetView>& renderTargetView)
{
	HRESULT hr = swapChain->GetBuffer(0, IID_PPV_ARGS(&renderTargetBuffer));

	if (FAILED(hr)) {
		throw std::runtime_error("Failed to call IDXGISwapChain::GetBuffer");
	}

	// Create RTV
	hr = device->CreateRenderTargetView(renderTargetBuffer.Get(), nullptr, &renderTargetView);

	if (FAILED(hr)) {
		throw std::runtime_error("Failed to create RTV");
	}
}

RenderTarget::RenderTarget(ID3D11Device* device, int32 width, int32 height)
{
	const DXGI_FORMAT surfaceFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
	const DXGI_FORMAT depthFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	BuildRenderTarget(device, surfaceFormat, width, height, 4, m_renderTargetBuffer, m_renderTargetView, m_shaderResourceView);
	BuildDepthBuffer(device, depthFormat, width, height, 4, m_depthStencilBuffer, m_depthStencilView);
}

RenderTarget::RenderTarget(ID3D11Device* device, IDXGISwapChain* swapChain, int32 width, int32 height)
{
	const DXGI_FORMAT depthFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	BuildBackBufferBySwapChain(device, swapChain, m_renderTargetBuffer, m_renderTargetView);
	BuildDepthBuffer(device, depthFormat, width, height, 4, m_depthStencilBuffer, m_depthStencilView);
}

ID3D11RenderTargetView* RenderTarget::getRenderTargetView() const
{
	return m_renderTargetView.Get();
}

ID3D11DepthStencilView* RenderTarget::getDepthStencilView() const
{
	return m_depthStencilView.Get();
}

ID3D11ShaderResourceView* RenderTarget::getShaderResourceView() const
{
	return m_shaderResourceView.Get();
}

} // namespace hks
