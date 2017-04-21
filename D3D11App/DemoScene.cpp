#include "DemoScene.h"

#include "SimpleMath.h"
using namespace DirectX::SimpleMath;
#include <d3dcompiler.h>
#include <stdexcept>

#pragma comment(lib, "d3dcompiler.lib")

namespace {
	struct MyVertex {
		Vector3 pos;
		Vector4 color;
	};
	struct ConstantBuffer {
		Matrix world;
		Matrix view;
		Matrix proj;
	};
}

DemoScene::DemoScene(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext) :
	m_device(device), m_deviceContext(deviceContext)
{
	createShader(L"./Shader/test.fx");
	createBuffer();
}

Scene* DemoScene::update()
{
	return this;
}

void DemoScene::draw()
{
	Matrix world, view, proj;
	D3D11_MAPPED_SUBRESOURCE resource;

	view = DirectX::XMMatrixLookAtLH({ 0, 0, -5 }, { 0, 0, 0 }, { 0, 1, 0 });
	proj = DirectX::XMMatrixPerspectiveFovLH(3.141592654f / 4.0f, 800 / 600, 1.0f, 1000.0f);

	//転置
	world = XMMatrixTranspose(world);
	view = XMMatrixTranspose(view);
	proj = XMMatrixTranspose(proj);

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

	//set input layout
	m_deviceContext->IASetInputLayout(m_layout.Get());
	//set shader
	m_deviceContext->VSSetShader(m_vertexShader.Get(), NULL, 0);
	m_deviceContext->PSSetShader(m_pixelShader.Get(), NULL, 0);

	unsigned int stride = sizeof(MyVertex);
	unsigned int offset = 0;
	m_deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
	m_deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_deviceContext->DrawIndexed(3, 0, 0);
}

void compileFromFile(WCHAR * filePath, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob ** blobOut)
{
	DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS;
	ComPtr<ID3DBlob> errorBlob;

	HRESULT hr = D3DCompileFromFile(filePath, NULL, NULL, entryPoint, shaderModel, flags, 0, blobOut, &errorBlob);

	if (FAILED(hr))
		throw std::runtime_error((char*)errorBlob->GetBufferPointer());
}

void DemoScene::createShader(WCHAR * filePath)
{
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
}

void DemoScene::createBuffer()
{
	//create vertex buffer
	{
		MyVertex vertices[] = {
			{{-1, -1, 0}, {0, 1, 1, 1}},
			{{0, 1, 0}, {0, 1, 1, 1}},
			{{1, -1, 0}, {0, 1, 1, 1}},
		};

		D3D11_BUFFER_DESC desc = {};
		desc.ByteWidth = sizeof(MyVertex) * 3;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA resource = {};
		resource.pSysMem = vertices;

		HRESULT hr = m_device->CreateBuffer(&desc, &resource, &m_vertexBuffer);
		if (FAILED(hr))
			throw std::runtime_error("CreateVertexBuffer Failed.");
	}

	//create index buffer
	{
		unsigned long indices[] = {0, 1, 2};

		D3D11_BUFFER_DESC desc = {};
		desc.ByteWidth = sizeof(unsigned long) * 3;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA resource = {};
		resource.pSysMem = indices;

		HRESULT hr = m_device->CreateBuffer(&desc, &resource, &m_indexBuffer);
		if (FAILED(hr))
			throw std::runtime_error("CreateIndexBuffer Failed.");
	}

	//create constant buffer
	{
		D3D11_BUFFER_DESC desc = {};
		desc.ByteWidth = sizeof(ConstantBuffer);
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		HRESULT hr = m_device->CreateBuffer(&desc, NULL, &m_constantBuffer);
		if (FAILED(hr))
			throw std::runtime_error("CreateConstantBuffer Failed.");
	}
}
