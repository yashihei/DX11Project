// -----------------------------------------------
// File : Model.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <vector>
#include <unordered_map>
#include <memory>
#include "DirectXTK/SimpleMath.h"
#include "DirectXTK/VertexTypes.h"
#include "Fwd.h"

using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;
using ModelVertex = DirectX::VertexPositionNormalTexture;

struct Material
{
	Color diffuse;
	Color ambient;
	Color specular;
	float power;
	std::string textureName;
};

class Mesh
{
public:
	Mesh(ComPtr<ID3D11Device> device, std::vector<ModelVertex>& vertices, std::vector<unsigned long>& indices, int materialID);
	void draw(ComPtr<ID3D11DeviceContext> deviceContext);
	int getMatID() const { return m_materialID; }
private:
	int m_indexCount, m_materialID;
	ComPtr<ID3D11Buffer> m_vertexBuffer;
	ComPtr<ID3D11Buffer> m_indexBuffer;
};

class Model
{
public:
	Model(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, CommonStatesPtr states);
	void createFromObj(const std::string& filePath);
	void draw(const Matrix& world, const Matrix& view, const Matrix& proj);
private:
	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_deviceContext;
	CommonStatesPtr m_states;
	std::vector<std::shared_ptr<Mesh>> m_meshes;
	std::vector<Material> m_materials;
	std::unordered_map<std::string, ComPtr<ID3D11ShaderResourceView>> m_textures;
	DirectX::BoundingSphere m_boundingSphere;
	BasicEffectPtr m_effect;
};
