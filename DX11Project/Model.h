// -----------------------------------------------
// File : Model.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <memory>
#include <vector>
#include "DirectXTK/SimpleMath.h"
#include "DirectXTK/VertexTypes.h"
#include "Fwd.h"
#include <unordered_map>

using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;

using ModelVertex = DirectX::VertexPositionNormalTexture;

class Model {
public:
	Model(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, CommonStatesPtr states);
	void createFromPmx(const std::string& filePath);
	void createFromObj(const std::string& filePath);
	void draw(const Matrix& world, const Matrix& view, const Matrix& proj);
private:
	struct Material {
		Color diffuse;
		Color ambient;
		Color specular;
		float power;
		std::string textureName;
		int indexCount;
	};

	void createVertexBuffer(const std::vector<ModelVertex>& vertices);
	void createIndexBuffer(const std::vector<unsigned long>& indices);

	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_deviceContext;
	CommonStatesPtr m_states;

	ComPtr<ID3D11Buffer> m_vertexBuffer;
	ComPtr<ID3D11Buffer> m_indexBuffer;
	std::vector<Material> m_materials;
	std::unordered_map<std::string, ComPtr<ID3D11ShaderResourceView>> m_textures;
	DirectX::BoundingSphere m_boundingSphere;
	BasicEffectPtr m_effect;
};
