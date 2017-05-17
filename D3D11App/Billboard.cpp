#include "Billboard.h"

#include "Camera.h"
#include "SpriteEffect.h"
#include <vector>
#include <stdexcept>

namespace {
	struct SpriteVertex {
		Vector3 pos;
		Vector2 uv;
		Vector4 color;
	};
}

Billboard::Billboard(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, ComPtr<ID3D11ShaderResourceView> texture, CameraPtr camera)
	: m_device(device), m_deviceContext(deviceContext), m_texture(texture), m_camera(camera)
{
	m_spriteEffect = std::make_shared<SpriteEffect>(m_device, m_deviceContext);
	m_spriteEffect->setTexture(m_texture);

	createVertexBuffer();
	createIndexBuffer();
}

void Billboard::draw(const Vector3& pos, float scale)
{
	unsigned int stride = sizeof(SpriteVertex);
	unsigned int offset = 0;
	m_deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
	m_deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	Matrix world;
	world *= Matrix::CreateScale(scale);
	Matrix view = m_camera->getViewMat();
	view._41 = view._42 = view._43 = 0;
	world *= view.Invert();
	world *= Matrix::CreateTranslation(pos);

	m_spriteEffect->setParam(world.Transpose(), m_camera->getViewMat().Transpose(), m_camera->getProjMat().Transpose());
	m_spriteEffect->apply();

	m_deviceContext->DrawIndexed(4, 0, 0);
}

void Billboard::createVertexBuffer()
{
	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(SpriteVertex) * 4;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	std::vector<SpriteVertex> vertices {
		{{-0.5f,  0.5f, 0.0f}, {0.0f, 0.0f}, {1, 1, 1, 1}},
		{{ 0.5f,  0.5f, 0.0f}, {1.0f, 0.0f}, {1, 1, 1, 1}},
		{{-0.5f, -0.5f, 0.0f}, {0.0f, 1.0f}, {1, 1, 1, 1}},
		{{ 0.5f, -0.5f, 0.0f}, {1.0f, 1.0f}, {1, 1, 1, 1}},
	};

	D3D11_SUBRESOURCE_DATA resource = {};
	resource.pSysMem = vertices.data();

	HRESULT hr = m_device->CreateBuffer(&desc, &resource, &m_vertexBuffer);
	if (FAILED(hr))
		throw std::runtime_error("CreateVertexBuffer Failed.");
}

void Billboard::createIndexBuffer()
{
	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(unsigned long) * 4;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.CPUAccessFlags = 0;

	unsigned long indices[] = { 0, 1, 2, 3 };
	D3D11_SUBRESOURCE_DATA resource = {};
	resource.pSysMem = indices;

	HRESULT hr = m_device->CreateBuffer(&desc, &resource, &m_indexBuffer);
	if (FAILED(hr))
		throw std::runtime_error("CreateIndexBuffer Failed.");
}
