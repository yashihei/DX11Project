#include "Model.h"

#include <stdexcept>
#include "MMD\Pmx.h"
#include "ShaderRV.h"

Model::Model(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext) :
	m_device(device), m_deviceContext(deviceContext),
	m_vertexCount(0), m_indexCount(0)
{
	m_vertexCount = 4, m_indexCount = 6;
	loadPmx();
	createVertexBuffer();
	createIndexBuffer();
}

void Model::draw()
{
	unsigned int stride = sizeof(MyVertex);
	unsigned int offset = 0;

	m_deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
	m_deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_deviceContext->DrawIndexed(m_indexCount, 0, 0);
}

void Model::loadPmx()
{
	//pmx load
	pmx::PmxModel pmxModel;

	std::filebuf fb;
	if (!fb.open("assets/alicia/Alicia_solid.pmx", std::ios::in | std::ios::binary)) {
		throw std::runtime_error("Pmx Load Failed.");
	}
	std::istream is(&fb);
	pmxModel.Read(&is);

	//make model data
	m_vertexCount = pmxModel.vertex_count;
	m_indexCount = pmxModel.index_count;

	for (int i = 0; i < m_vertexCount; i++) {
		Vector3 pos = {
			pmxModel.vertices[i].positon[0],
			pmxModel.vertices[i].positon[1],
			pmxModel.vertices[i].positon[2]
		};
		Vector2 uv = {
			pmxModel.vertices[i].uv[0],
			pmxModel.vertices[i].uv[1]
		};
		Vector3 normal = {
			pmxModel.vertices[i].normal[0],
			pmxModel.vertices[i].normal[1],
			pmxModel.vertices[i].normal[2]
		};

		//FIXME:余計なコピー
		MyVertex vtx;
		vtx.pos = pos, vtx.uv = uv, vtx.normal = normal;
		m_vertices.push_back(vtx);
	}
	for (int i = 0; i < m_indexCount; i++) {
		m_indices.emplace_back(pmxModel.indices[i]);
	}

}

void Model::createVertexBuffer()
{
	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = sizeof(MyVertex) * m_vertexCount;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA resource = {};
	resource.pSysMem = m_vertices.data();

	HRESULT hr = m_device->CreateBuffer(&desc, &resource, &m_vertexBuffer);
	if (FAILED(hr))
		throw std::runtime_error("CreateVertexBuffer Failed.");
}

void Model::createIndexBuffer()
{
	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA resource = {};
	resource.pSysMem = m_indices.data();

	HRESULT hr = m_device->CreateBuffer(&desc, &resource, &m_indexBuffer);
	if (FAILED(hr))
		throw std::runtime_error("CreateIndexBuffer Failed.");
}
