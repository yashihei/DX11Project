#include "Graphics.h"

#include <stdexcept>

Graphics::Graphics()
{
}

Graphics::~Graphics()
{
}

void Graphics::initialize(int screenWidth, int screenHeight, HWND hWnd, bool fullScreen)
{
	if (!createDeviceAndSwapChain(screenWidth, screenHeight, hWnd, fullScreen))
		throw std::runtime_error("Error Create DeviceAndSwapChain.");

	if (!createRenderTarget())
		throw std::runtime_error("Error Create RenderTarget.");

	if (!createDepthStencil(screenWidth, screenHeight))
		throw std::runtime_error("Error Create DepthStencil");

	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

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
	float color[] = {0.1f, 0.1f, 0.1f, 1};
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1, 0);
}

void Graphics::endScene()
{
	m_swapChain->Present(0, 0);
}

bool Graphics::createDeviceAndSwapChain(int screenWidth, int screenHeight, HWND hWnd, bool fullScreen)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;
	//MEMO: format, refreshrate�͉��u��
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hWnd;
	//�}���`�T���v�����OOFF
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = fullScreen ? false : true;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		createDeviceFlags,
		&featureLevel,
		1,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&m_swapChain,
		&m_device,
		NULL,
		&m_deviceContext
	);
	if (FAILED(hr))
		return false;

	return true;
}

bool Graphics::createRenderTarget()
{
	ID3D11Texture2D* backBuffer;
	HRESULT hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
	if (FAILED(hr))
		return false;

	hr = m_device->CreateRenderTargetView(backBuffer, NULL, &m_renderTargetView);
	if (FAILED(hr))
		return false;

	backBuffer->Release();
	return true;
}

bool Graphics::createDepthStencil(int screenWidth, int screenHeight)
{
	D3D11_TEXTURE2D_DESC depthDesc = {};
	depthDesc.Width = screenWidth;
	depthDesc.Height = screenHeight;
	depthDesc.MipLevels = 1;
	depthDesc.ArraySize = 1;
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.SampleDesc.Count = 1;
	depthDesc.SampleDesc.Quality = 0;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDesc.CPUAccessFlags = 0;
	depthDesc.MiscFlags = 0;

	HRESULT hr = m_device->CreateTexture2D(&depthDesc, NULL, &m_depthStencilBuffer);
	if (FAILED(hr))
		return false;

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = depthDesc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	//multisample > 0�̏ꍇ
	//dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

	hr = m_device->CreateDepthStencilView(m_depthStencilBuffer, &dsvDesc, &m_depthStencilView);
	if (FAILED(hr))
		return false;

	return true;
}
