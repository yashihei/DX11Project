// -----------------------------------------------
// File : Model.cpp
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#include "Model.h"

#include "ShaderRV.h"
#include "BasicEffect.h"
#include "Camera.h"
#include "LightParam.h"
#include "UtilStr.h"
#include <DirectXTK/CommonStates.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include <ObjLoader/tiny_obj_loader.h>

Mesh::Mesh(ComPtr<ID3D11Device> device, std::vector<ModelVertex>& vertices, std::vector<unsigned long>& indices, int materialID) :
	m_indexCount(indices.size()), m_materialID(materialID)
{
	//Create VertexBuffer
	{
		D3D11_BUFFER_DESC desc = {};
		desc.ByteWidth = sizeof(ModelVertex) * vertices.size();
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA resource = {};
		resource.pSysMem = vertices.data();

		HRESULT hr = device->CreateBuffer(&desc, &resource, &m_vertexBuffer);
		if (FAILED(hr))
			throw std::runtime_error("CreateVertexBuffer Failed.");
	}

	//Create IndexBuffer
	{
		D3D11_BUFFER_DESC desc = {};
		desc.ByteWidth = sizeof(unsigned long) * indices.size();
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA resource = {};
		resource.pSysMem = indices.data();

		HRESULT hr = device->CreateBuffer(&desc, &resource, &m_indexBuffer);
		if (FAILED(hr))
			throw std::runtime_error("CreateIndexBuffer Failed.");
	}
}

void Mesh::draw(ComPtr<ID3D11DeviceContext> deviceContext)
{
	unsigned int stride = sizeof(ModelVertex);
	unsigned int offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
	deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->DrawIndexed(m_indexCount, 0, 0);
}

Model::Model(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, CommonStatesPtr states, CameraPtr camera, LightParamPtr light) :
	m_device(device), m_deviceContext(deviceContext), m_states(states), m_camera(camera), m_light(light)
{
	m_effect = std::make_shared<BasicEffect>(m_device, m_deviceContext);
}

void Model::createFromObj(const std::string& filePath)
{
	if (StrCmp(".obj", PathFindExtension(filePath.c_str())))
		throw std::runtime_error(filePath + " isn't obj file.");

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filePath.c_str(), GetDirPath(filePath).c_str());
	if (!err.empty()) {
		throw std::runtime_error(filePath + " Load Failed.");
	}

	//Loop over shapes (g or o)
	for (unsigned int s = 0; s < shapes.size(); s++) {
		std::vector<ModelVertex> vertices;
		std::vector<unsigned long> indices;
		int materialID = shapes[s].mesh.material_ids[0];
		int indexOffset = 0, indexCount = 0;

		//Loop over faces
		for (unsigned int f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			//マテリアルが異なる場合meshを分割する
			if (materialID != shapes[s].mesh.material_ids[f]) {
				auto mesh = std::make_shared<Mesh>(m_device, vertices, indices, materialID);
				m_meshes.push_back(mesh);
				//reset
				vertices.clear();
				indices.clear();
				indexCount = 0;
			}

			//Loop over vertices in the
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

				vertices.emplace_back(pos, normal, texCoord);
				indices.push_back(indexCount + v);
			}

			materialID = shapes[s].mesh.material_ids[f];
			indexOffset += fv;
			indexCount += fv;
		}

		//グループ毎にメッシュを作る
		auto mesh = std::make_shared<Mesh>(m_device, vertices, indices, materialID);
		m_meshes.push_back(mesh);
	}

	//Calc bounds
	std::vector<Vector3> points;
	for (unsigned int i = 0; i < attrib.vertices.size() / 3; i++) {
		points.emplace_back(attrib.vertices[3 * i + 0], attrib.vertices[3 * i + 1], attrib.vertices[3 * i + 2]);
	}
	DirectX::BoundingSphere::CreateFromPoints(m_boundingSphere, points.size(), points.data(), sizeof(Vector3));

	//Load texures
	const auto nulltex = CreateShaderResourceViewFromFile(m_device, L"assets/null.png");
	m_textures["nulltex"] = nulltex;

	const auto rootDir = GetDirPath(filePath);
	for (unsigned int i = 0; i < materials.size(); i++) {
		const auto texName = materials[i].diffuse_texname;
		if (!texName.empty()) {
			const auto dir = rootDir + texName;
			const auto tex = CreateShaderResourceViewFromFile(m_device, s2ws(dir).c_str());
			m_textures[texName] = tex;
		}
	}

	//Load material
	for (unsigned int i = 0; i < materials.size(); i++) {
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
		};
		m_materials.push_back(mat);
	}
}

void Model::draw(const Vector3& pos, const Vector3& rot, const Vector3& scale)
{
	const Matrix world = Matrix::CreateScale(scale) * Matrix::CreateFromYawPitchRoll(rot.y, rot.x, rot.z) * Matrix::CreateTranslation(pos);
	const Matrix view = m_camera->getViewMat();
	const Matrix proj = m_camera->getProjMat();
	draw(world, view, proj);
}

void Model::draw(const Matrix& world, const Matrix& view, const Matrix& proj)
{
	auto samplerState = m_states->LinearWrap();
	m_deviceContext->PSSetSamplers(0, 1, &samplerState);

	m_effect->setLightParams(m_light->diffuse, m_light->ambient, m_light->direction);
	m_effect->setObjectParams(world.Transpose(), view.Transpose(), proj.Transpose());

	for (auto& mesh : m_meshes) {
		auto mat = m_materials[mesh->getMatID()];

		if (!mat.textureName.empty())
			m_effect->setTexture(m_textures[mat.textureName]);
		else
			m_effect->setTexture(m_textures["nulltex"]);

		m_effect->setMaterialParams(mat.diffuse, mat.ambient, mat.specular, mat.power);
		m_effect->apply();

		mesh->draw(m_deviceContext);
	}
}
