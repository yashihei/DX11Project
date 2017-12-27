// -----------------------------------------------
// File : RenderTarget.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include "Types.h"

namespace hks {

using Microsoft::WRL::ComPtr;

class RenderTarget {
public:
	RenderTarget(ID3D11Device* device, int32 width, int32 height, int32 multiSampleCount);
	RenderTarget(ID3D11Device* device, IDXGISwapChain* swapChain, int32 width, int32 height, int32 multiSampleCount);
	ID3D11RenderTargetView* getRenderTargetView() const;
	ID3D11DepthStencilView* getDepthStencilView() const;
	ID3D11ShaderResourceView* getShaderResourceView() const;
private:
	ComPtr<ID3D11Texture2D> m_renderTargetBuffer;
	ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
	ComPtr<ID3D11RenderTargetView> m_renderTargetView;
	ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;
};

} // namespace hks
