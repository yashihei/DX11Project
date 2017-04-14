#pragma once

//Lib linked
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

#include <dxgi.h>
#include <d3d11.h>

class Graphics {
public:
	Graphics();
	~Graphics();
	void initialize(int screenWidth, int screenHeight, HWND hWnd, bool fullScreen);
	void beginScene();
	void endScene();
	ID3D11Device* getDevice() const { return m_device; }
	ID3D11DeviceContext* getDeviceContext() { return m_deviceContext; }
private:
	bool createDeviceAndSwapChain(int screenWidth, int screenHeight, HWND hWnd, bool fullScreen);
	bool createRenderTarget();
	bool createDepthStencil(int screenWidth, int screenHeight);

	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	IDXGISwapChain* m_swapChain;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11Texture2D* m_depthStencilBuffer;
};
