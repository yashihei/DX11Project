#pragma once

#include <d3d11.h>
#include <wrl\client.h>
#include <memory>
#include <vector>
#include "SimpleMath.h"

using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;

class Model {
public:
	Model(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
	void draw();
private:
	struct MyVertex {
		Vector3 pos;
		Vector2 uv;
		Vector3 normal;
	};
	void loadPmx();
	void createVertexBuffer();
	void createIndexBuffer();
	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_deviceContext;
	ComPtr<ID3D11Buffer> m_vertexBuffer;
	ComPtr<ID3D11Buffer> m_indexBuffer;
	ComPtr<ID3D11ShaderResourceView> m_resource;

	UINT m_vertexCount, m_indexCount;
	std::vector<MyVertex> m_vertices;
	std::vector<unsigned long> m_indices;
};

using ModelPtr = std::shared_ptr<Model>;
