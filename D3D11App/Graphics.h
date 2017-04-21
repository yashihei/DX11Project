#pragma once

//Lib linked
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

#include <dxgi.h>
#include <d3d11.h>

#include <wrl\client.h>
using Microsoft::WRL::ComPtr;

class Graphics {
public:
	Graphics(int screenWidth, int screenHeight, HWND hWnd, bool fullScreen);
	void beginScene();
	void endScene();
	ComPtr<ID3D11Device> getDevice() const { return m_device; }
	ComPtr<ID3D11DeviceContext> getDeviceContext() const { return m_deviceContext; }
private:
	bool createDeviceAndSwapChain(int screenWidth, int screenHeight, HWND hWnd, bool fullScreen);
	bool createRenderTarget();
	bool createDepthStencil(int screenWidth, int screenHeight);
	bool createRasterizerState();

	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_deviceContext;
	ComPtr<IDXGISwapChain> m_swapChain;
	ComPtr<ID3D11RenderTargetView> m_renderTargetView;
	ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
};
