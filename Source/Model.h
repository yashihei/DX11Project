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
#include <DirectXTK/SimpleMath.h>
#include <DirectXTK/VertexTypes.h>
#include "Fwd.h"
#include "PtrAlias.h"
#include "MathAlias.h"
#include "Types.h"

namespace hks {

using Microsoft::WRL::ComPtr;
using ModelVertex = DirectX::VertexPositionNormalTexture;

struct Material {
	Color diffuse;
	Color ambient;
	Color specular;
	float power;
	std::string textureName;
};

class Mesh {
public:
	Mesh(ComPtr<ID3D11Device> device, std::vector<ModelVertex>& vertices, std::vector<unsigned long>& indices, int32 materialID);
	void draw(ComPtr<ID3D11DeviceContext> deviceContext);
	int32 getMatID() const { return m_materialID; }
private:
	int32 m_indexCount, m_materialID;
	ComPtr<ID3D11Buffer> m_vertexBuffer;
	ComPtr<ID3D11Buffer> m_indexBuffer;
};

class Model {
public:
	Model(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, CameraPtr camera, LightParamPtr light);
	void createFromObj(const std::string& filePath);
	void draw(const Vector3& pos, const Quaternion& rot, const Vector3& scale = Vector3::One);
	void draw(const Vector3& pos, const Vector3& rot = Vector3::Zero, const Vector3& scale = Vector3::One);
	void draw(const Matrix& world, const Matrix& view, const Matrix& proj);
private:
	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_deviceContext;
	CameraPtr m_camera;
	LightParamPtr m_light;
	std::vector<std::shared_ptr<Mesh>> m_meshes;
	std::vector<Material> m_materials;
	std::unordered_map<std::string, ComPtr<ID3D11ShaderResourceView>> m_textures;
	DirectX::BoundingSphere m_boundingSphere;
	ToonEffectPtr m_effect;
};

} // namespace hks
