// -----------------------------------------------
// File : Model.cpp
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#include "Model.h"

#include <algorithm>
#include <stdexcept>
#include "MMD/Pmx.h"
#include "ShaderRV.h"
#include "Utility.h"
#include "BasicEffect.h"
#include "ToonEffect.h"
#include "DirectXTK/CommonStates.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "ObjLoader/tiny_obj_loader.h"
#undef min
#undef max

Model::Model(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, CommonStatesPtr states) :
	m_device(device), m_deviceContext(deviceContext), m_states(states)
{
	m_effect = std::make_shared<BasicEffect>(m_device, m_deviceContext);
}

void Model::draw(const Matrix& world, const Matrix& view, const Matrix& proj)
{
	if (m_vertices.size() == 0) {
		throw std::runtime_error("Model uninit.");
	}

	// setsampler
	auto samplerState = m_states->LinearWrap();
	m_deviceContext->PSSetSamplers(0, 1, &samplerState);

	// set buffers
	unsigned int stride = sizeof(ModelVertex);
	unsigned int offset = 0;
	m_deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
	m_deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	// set topology
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_effect->setLightParams({ 1, 1, 1, 1 }, { 0.3f, 0.3f, 0.3f, 1 }, { 0, 0, 1 });
	m_effect->setObjectParams(world.Transpose(), view.Transpose(), proj.Transpose());

	int countIndex = 0;
	for (const auto& mat : m_materials) {
		if (!mat.textureName.empty()) {
			m_effect->setTexture(m_textures[mat.textureName]);
		} else {
			m_effect->setTexture(m_textures["nulltex"]);
		}
		m_effect->setMaterialParams(mat.diffuse, mat.ambient, mat.specular, mat.power);
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
		const Vector3 pos {
			pmxModel.vertices[i].positon[0],
			pmxModel.vertices[i].positon[1],
			pmxModel.vertices[i].positon[2]
		};
		const Vector3 normal {
			pmxModel.vertices[i].normal[0],
			pmxModel.vertices[i].normal[1],
			pmxModel.vertices[i].normal[2]
		};
		const Vector2 uv {
			pmxModel.vertices[i].uv[0],
			pmxModel.vertices[i].uv[1]
		};
		m_vertices.emplace_back(pos, normal, uv);
	}

	//load idx
	for (int i = 0; i < pmxModel.index_count; i++) {
		m_indices.push_back(pmxModel.indices[i]);
	}
	
	//load tex
	const auto nulltex = CreateShaderResourceViewFromFile(m_device, L"assets/null.png");
	m_textures["nulltex"] = nulltex;

	const auto rootDir = GetDirPath(filePath);
	for (int i = 0; i < pmxModel.texture_count; i++) {
		const auto dir = s2ws(rootDir) + pmxModel.textures[i];
		const auto tex = CreateShaderResourceViewFromFile(m_device, dir.c_str());
		m_textures[ws2s(pmxModel.textures[i])] = tex;
	}

	//load material
	for (int i = 0; i < pmxModel.material_count; i++) {
		const int texIdx = pmxModel.materials[i].diffuse_texture_index;
		const std::string texName = (texIdx != -1) ? ws2s(pmxModel.textures[texIdx]) : "";

		const Material mat {
			{
				pmxModel.materials[i].diffuse[0],
				pmxModel.materials[i].diffuse[1],
				pmxModel.materials[i].diffuse[2],
				pmxModel.materials[i].diffuse[3]
			},
			{
				pmxModel.materials[i].ambient[0],
				pmxModel.materials[i].ambient[1],
				pmxModel.materials[i].ambient[2],
				pmxModel.materials[i].ambient[3],
			},
			{
				pmxModel.materials[i].specular[0],
				pmxModel.materials[i].specular[1],
				pmxModel.materials[i].specular[2],
				pmxModel.materials[i].specular[3],
			},
			pmxModel.materials[i].specularlity,
			texName,
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

	std::vector<int> indexCount;
	indexCount.resize(materials.size());

	for (int s = 0; s < shapes.size(); s++) {
		int indexOffset = 0;
		for (int f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			//loop face
			const int fv = shapes[s].mesh.num_face_vertices[f];
			for (int v = 0; v < fv; v++) {
				const auto idx = shapes[s].mesh.indices[indexOffset + v];

				Vector3 pos = {};
				pos.x = attrib.vertices[3 * idx.vertex_index + 0];
				pos.y = attrib.vertices[3 * idx.vertex_index + 1];
				pos.z = attrib.vertices[3 * idx.vertex_index + 2];

				Vector3 normal = {};
				if (attrib.normals.size() > 0) {
					normal.x = attrib.normals[3 * idx.normal_index + 0];
					normal.y = attrib.normals[3 * idx.normal_index + 1];
					normal.z = attrib.normals[3 * idx.normal_index + 2];
				} else {
					//TODO: calc normal
				}

				Vector2 texCoord = {};
				if (attrib.texcoords.size() > 0) {
					texCoord.x = attrib.texcoords[2 * idx.texcoord_index + 0];
					texCoord.y = attrib.texcoords[2 * idx.texcoord_index + 1];
				}

				m_vertices.emplace_back(pos, normal, texCoord);
				m_indices.push_back(indexOffset + v);
			}
			indexOffset += fv;

			indexCount[shapes[s].mesh.material_ids[f]] += fv;
		}
	}

	//TODO:load tex
	const auto nulltex = CreateShaderResourceViewFromFile(m_device, L"assets/null.png");
	m_textures["nulltex"] = nulltex;

	const auto rootDir = GetDirPath(filePath);
	for (int i = 0; i < materials.size(); i++) {
		const auto texName = materials[i].diffuse_texname;
		if (!texName.empty())
		{
			const auto dir = rootDir + texName;
			const auto tex = CreateShaderResourceViewFromFile(m_device, s2ws(dir).c_str());
			m_textures[texName] = tex;
		}
	}

	//load materials
	for (int i = 0; i < materials.size(); i++) {
		const Material mat {
			{
				materials[i].diffuse[0],
				materials[i].diffuse[1],
				materials[i].diffuse[2],
				materials[i].dissolve
			},
			{
				materials[i].ambient[0],
				materials[i].ambient[1],
				materials[i].ambient[2],
				materials[i].dissolve
			},
			{
				materials[i].specular[0],
				materials[i].specular[1],
				materials[i].specular[2],
				materials[i].dissolve
			},
			materials[i].shininess,
			materials[i].diffuse_texname,
			indexCount[i],
		};
		m_materials.push_back(mat);
	}

	createVertexBuffer();
	createIndexBuffer();
}

void Model::getBoudingSphere(Vector3 * center, float * r)
{
	if (m_vertices.size() == 0) {
		throw std::runtime_error("Model uninit.");
	}

	// calc center pos
	// --------------------------------------------------
	Vector3 maxPos;
	Vector3 minPos = maxPos = m_vertices[0].position;

	for (const auto& vertex : m_vertices) {
		//min
		minPos.x = std::min(minPos.x, vertex.position.x);
		minPos.y = std::min(minPos.y, vertex.position.y);
		minPos.z = std::min(minPos.z, vertex.position.z);
		//max
		maxPos.x = std::max(maxPos.x, vertex.position.x);
		maxPos.y = std::max(maxPos.y, vertex.position.y);
		maxPos.z = std::max(maxPos.z, vertex.position.z);
	}

	*center = (maxPos + minPos) / 2;

	// calc radius
	// --------------------------------------------------
	float maxDistance = 0;

	for (const auto& vertex : m_vertices) {
		//right?
		float distance = Vector3::Distance(*center, vertex.position);
		maxDistance = std::max(maxDistance, distance);
	}

	*r = maxDistance;
}

void Model::getBoudingSphere(DirectX::BoundingSphere& sphere)
{
	std::vector<Vector3> points;
	for (const auto& vertex : m_vertices) {
		points.push_back(vertex.position);
	}

	DirectX::BoundingSphere::CreateFromPoints(sphere, points.size(), points.data(), sizeof(Vector3));
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
