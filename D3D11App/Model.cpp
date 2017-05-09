#include "Model.h"

#include <stdexcept>
#include "MMD/Pmx.h"
#include "ShaderRV.h"
#include "Utility.h"
#include "BasicEffect.h"

Model::Model(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext) :
	m_device(device), m_deviceContext(deviceContext)
{
	m_effect = std::make_shared<BasicEffect>(m_device, m_deviceContext);
}

void Model::draw(const Matrix& world, const Matrix& view, const Matrix& proj)
{
	if (m_vertices.size() == 0) {
		throw std::runtime_error("Model uninit.");
	}

	unsigned int stride = sizeof(ModelVertex);
	unsigned int offset = 0;
	m_deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
	m_deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_effect->setParam(world, view, proj, { 0, 0, 1 }, { 1, 1, 1, 1 }, { 0.5f, 0.5f, 0.5f, 1.0f });

	int countIndex = 0;
	for (const auto& mat : m_materials) {
		if (mat.diffuseTexureIndex == -1) continue;//FIXME
		m_effect->setTexture(m_textures[mat.diffuseTexureIndex]);
		m_effect->apply();
		m_deviceContext->DrawIndexed(mat.indexCount, countIndex, 0);
		countIndex += mat.indexCount;
	}
}

void Model::createFromPmx(const std::string& filePath)
{
	pmx::PmxModel pmxModel;
	std::filebuf fb;
	if (!fb.open(filePath, std::ios::in | std::ios::binary)) {
		throw std::runtime_error(filePath + " Load Failed.");
	}
	std::istream is(&fb);
	pmxModel.Read(&is);

	//load vtx
	for (int i = 0; i < pmxModel.vertex_count; i++) {
		Vector3 pos {
			pmxModel.vertices[i].positon[0],
			pmxModel.vertices[i].positon[1],
			pmxModel.vertices[i].positon[2]
		};
		Vector2 uv {
			pmxModel.vertices[i].uv[0],
			pmxModel.vertices[i].uv[1]
		};
		Vector3 normal {
			pmxModel.vertices[i].normal[0],
			pmxModel.vertices[i].normal[1],
			pmxModel.vertices[i].normal[2]
		};
		m_vertices.push_back({ pos, uv, normal });
	}

	//load idx
	for (int i = 0; i < pmxModel.index_count; i++) {
		m_indices.push_back(pmxModel.indices[i]);
	}

	//load tex
	const auto rootDir = GetDirPath(filePath);
	for (int i = 0; i < pmxModel.texture_count; i++) {
		const auto dir = s2ws(rootDir) + pmxModel.textures[i];
		auto tex = CreateShaderResourceViewFromFile(m_device, dir.c_str());
		m_textures.push_back(tex);
	}

	//load material
	for (int i = 0; i < pmxModel.material_count; i++) {
		Material mat {
			{
				pmxModel.materials[i].diffuse[0],
				pmxModel.materials[i].diffuse[1],
				pmxModel.materials[i].diffuse[2],
				pmxModel.materials[i].diffuse[3]
			},
			pmxModel.materials[i].diffuse_texture_index,
			pmxModel.materials[i].index_count,
		};
		m_materials.push_back(mat);
	}

	createVertexBuffer();
	createIndexBuffer();
}

void Model::createFromObj(const std::string & filePath)
{
}

void Model::createVertexBuffer()
{
	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = sizeof(ModelVertex) * m_vertices.size();
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
	desc.ByteWidth = sizeof(unsigned long) * m_indices.size();
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA resource = {};
	resource.pSysMem = m_indices.data();

	HRESULT hr = m_device->CreateBuffer(&desc, &resource, &m_indexBuffer);
	if (FAILED(hr))
		throw std::runtime_error("CreateIndexBuffer Failed.");
}
