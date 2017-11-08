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
	void getBoudingSphere(Vector3* center, float* r);
	void getBoudingSphere(DirectX::BoundingSphere& sphere);
private:
	struct Material {
		Color diffuse;
		Color ambient;
		Color specular;
		float power;
		std::string textureName;
		int indexCount;
	};

	void createVertexBuffer();
	void createIndexBuffer();

	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_deviceContext;
	ComPtr<ID3D11Buffer> m_vertexBuffer;
	ComPtr<ID3D11Buffer> m_indexBuffer;

	std::vector<ModelVertex> m_vertices;
	std::vector<unsigned long> m_indices;
	std::unordered_map<std::string, ComPtr<ID3D11ShaderResourceView>> m_textures;
	std::vector<Material> m_materials;

	BasicEffectPtr m_effect;
	CommonStatesPtr m_states;
};
