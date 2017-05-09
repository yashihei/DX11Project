#include "Model.h"

#include <stdexcept>
#include "MMD/Pmx.h"
#include "ShaderRV.h"
#include "Utility.h"
#include "BasicEffect.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "ObjLoader/tiny_obj_loader.h"

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
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filePath.c_str(), GetDirPath(filePath).c_str());
	if (!err.empty()) {
		throw std::runtime_error(filePath + " Load Failed.");
	}

	//// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) {
		size_t index_offset = 0;
		// Loop over faces(polygon)
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			int fv = shapes[s].mesh.num_face_vertices[f];
			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

				tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
				tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
				tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];

				if (attrib.normals.size() == 0) break;
				tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
				tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
				tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];

				if (attrib.texcoords.size() == 0) break;
				tinyobj::real_t tx = attrib.texcoords[2 * idx.texcoord_index + 0];
				tinyobj::real_t ty = attrib.texcoords[2 * idx.texcoord_index + 1];
			}
			index_offset += fv;

			// per-face material
			shapes[s].mesh.material_ids[f];
		}
	}
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
