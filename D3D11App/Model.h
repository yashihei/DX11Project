#pragma once

#include <d3d11.h>
#include <wrl\client.h>
#include <memory>

using Microsoft::WRL::ComPtr;

class Model {
public:
	Model(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
	void draw();
private:
	void createVertexBuffer();
	void createIndexBuffer();
private:
	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_deviceContext;
	ComPtr<ID3D11Buffer> m_vertexBuffer;
	ComPtr<ID3D11Buffer> m_indexBuffer;
};

using ModelPtr = std::shared_ptr<Model>;
