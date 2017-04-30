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
	Model(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, EffectPtr effect);
	void createFromPmx(const std::string& filePath);
	//void createFromObj(const std::string& filePath);
	void draw();
private:
	struct MyVertex {
		Vector3 pos;
		Vector2 uv;
		Vector3 normal;
	};
	struct Material {
		Color diffuse;//and... specular, ambient, power;
		int diffuseTexureIndex;//and... sphereTexureIndex, toonTexureIndex;
		int indexCount;
	};

	void createVertexBuffer();
	void createIndexBuffer();

	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_deviceContext;
	ComPtr<ID3D11Buffer> m_vertexBuffer;
	ComPtr<ID3D11Buffer> m_indexBuffer;
	ComPtr<ID3D11ShaderResourceView> m_resource;

	std::vector<MyVertex> m_vertices;
	std::vector<unsigned long> m_indices;
	std::vector<ComPtr<ID3D11ShaderResourceView>> m_textures;
	std::vector<Material> m_materials;

	EffectPtr m_effect;
};
