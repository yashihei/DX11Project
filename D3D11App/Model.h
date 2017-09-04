#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <memory>
#include <vector>
#include "DirectXTK/SimpleMath.h"
#include "Fwd.h"

using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;

class Model {
public:
	Model(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
	void createFromPmx(const std::string& filePath);
	void createFromObj(const std::string& filePath);
	void getBoudingSphere(Vector3* center, float* r);
	void draw(const Matrix& world, const Matrix& view, const Matrix& proj);
private:
	struct ModelVertex {
		Vector3 pos;
		Vector2 uv;
		Vector3 normal;
	};

	struct Material {
		Color diffuse;
		Color ambient;
		Color specular;
		float power;
		int diffuseTexureIndex;//and... sphereTexureIndex, toonTexureIndex;
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
	std::vector<ComPtr<ID3D11ShaderResourceView>> m_textures;
	std::vector<Material> m_materials;

	BasicEffectPtr m_effect;
};
