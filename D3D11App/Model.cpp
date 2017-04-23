#include "Model.h"

#include <stdexcept>
#include "SimpleMath.h"

using namespace DirectX::SimpleMath;

namespace {
	struct MyVertex {
		Vector3 pos;
		Vector2 uv;
	};
}

Model::Model(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext) :
	m_device(device), m_deviceContext(deviceContext)
{
	createVertexBuffer();
	createIndexBuffer();
}

void Model::draw() {
	unsigned int stride = sizeof(MyVertex);
	unsigned int offset = 0;

	m_deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
	m_deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_deviceContext->DrawIndexed(6, 0, 0);
}

void Model::createVertexBuffer()
{
	MyVertex vertices[] = {
		{{-1, -1, 0}, {0, 0}},
		{{-1, 1, 0}, {1, 0}},
		{{1, 1, 0}, {1, 1}},
		{{1, -1, 0}, {0, 1}},
	};

	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = sizeof(MyVertex) * 4;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA resource = {};
	resource.pSysMem = vertices;

	HRESULT hr = m_device->CreateBuffer(&desc, &resource, &m_vertexBuffer);
	if (FAILED(hr))
		throw std::runtime_error("CreateVertexBuffer Failed.");
}

void Model::createIndexBuffer()
{
	unsigned long indices[] = {0, 1, 2, 0, 2, 3};

	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = sizeof(unsigned long) * 6;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA resource = {};
	resource.pSysMem = indices;

	HRESULT hr = m_device->CreateBuffer(&desc, &resource, &m_indexBuffer);
	if (FAILED(hr))
		throw std::runtime_error("CreateIndexBuffer Failed.");
}
