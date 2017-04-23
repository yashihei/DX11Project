#include "Effect.h"

#include <d3dcompiler.h>
#include <stdexcept>

#pragma comment(lib, "d3dcompiler.lib")

namespace {
	struct ConstantBuffer {
		Matrix world;
		Matrix view;
		Matrix proj;
	};
}

void compileFromFile(WCHAR * filePath, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob ** blobOut)
{
	DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS;
	ComPtr<ID3DBlob> errorBlob;

	HRESULT hr = D3DCompileFromFile(filePath, NULL, NULL, entryPoint, shaderModel, flags, 0, blobOut, &errorBlob);

	if (FAILED(hr))
		throw std::runtime_error((char*)errorBlob->GetBufferPointer());
}

Effect::Effect(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext) :
	m_device(device), m_deviceContext(deviceContext)
{
	WCHAR* filePath = L"./Shader/test.fx";

	//create vs
	ComPtr<ID3DBlob> VSbuffer;
	compileFromFile(filePath, "VS", "vs_5_0", &VSbuffer);
	HRESULT hr = m_device->CreateVertexShader(VSbuffer->GetBufferPointer(), VSbuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(hr))
		throw std::runtime_error("CreateVertexShader() Failed.");
	
	//create ps
	ComPtr<ID3DBlob> PSBuffer;
	compileFromFile(filePath, "PS", "ps_5_0", &PSBuffer);
	hr = m_device->CreatePixelShader(PSBuffer->GetBufferPointer(), PSBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(hr))
		throw std::runtime_error("CreatePixelShader() Failed.");

	//create input layout
	const D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	const UINT numElement = ARRAYSIZE(layout);

	hr = m_device->CreateInputLayout(layout, numElement, VSbuffer->GetBufferPointer(), VSbuffer->GetBufferSize(), &m_layout);
	if (FAILED(hr))
		throw std::runtime_error("CreateInputLayout() Failed.");

	//create constant buf
	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = sizeof(ConstantBuffer);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = m_device->CreateBuffer(&desc, NULL, &m_constantBuffer);
	if (FAILED(hr))
		throw std::runtime_error("CreateConstantBuffer Failed.");
}

void Effect::updateConstantBuffer(const Matrix& world, const Matrix& view, const Matrix& proj)
{
	D3D11_MAPPED_SUBRESOURCE resource;

	//コンスタントバッファ書き換え
	HRESULT hr = m_deviceContext->Map(m_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	if (SUCCEEDED(hr)) {
		ConstantBuffer* data = reinterpret_cast<ConstantBuffer*>(resource.pData);
		data->world = world;
		data->view = view;
		data->proj = proj;
		m_deviceContext->Unmap(m_constantBuffer.Get(), 0);
	}

	m_deviceContext->VSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());
}

void Effect::set()
{
	//set input layout
	m_deviceContext->IASetInputLayout(m_layout.Get());
	//set shader
	m_deviceContext->VSSetShader(m_vertexShader.Get(), NULL, 0);
	m_deviceContext->PSSetShader(m_pixelShader.Get(), NULL, 0);
}
