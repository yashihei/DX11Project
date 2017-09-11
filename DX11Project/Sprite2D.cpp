#include "Sprite2D.h"

#include <vector>
#include <stdexcept>
#include "SpriteEffect.h"

namespace {
	struct SpriteVertex {
		Vector3 pos;
		Vector2 uv;
		Vector4 color;
	};
}

Sprite2D::Sprite2D(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, ComPtr<ID3D11ShaderResourceView> texture)
	: m_device(device), m_deviceContext(deviceContext), m_texture(texture)
{
	//get tex desc
	ComPtr<ID3D11Resource> resource;
	m_texture->GetResource(&resource);
	ComPtr<ID3D11Texture2D> texture2D;
	resource.As(&texture2D);
	D3D11_TEXTURE2D_DESC desc;
	texture2D->GetDesc(&desc);

	m_size.x = static_cast<float>(desc.Width);
	m_size.y = static_cast<float>(desc.Height);

	//set offset
	UINT vpCount = 1;
	D3D11_VIEWPORT vp = {};
	m_deviceContext->RSGetViewports(&vpCount, &vp);
	m_offset.x = vp.Width / 2.0f; m_offset.y = vp.Height / 2.0f;

	const Matrix orthoMat = DirectX::XMMatrixOrthographicLH(vp.Width, vp.Height, 0, 1000.0f);
	m_spriteEffect = std::make_shared<SpriteEffect>(m_device, m_deviceContext);
	m_spriteEffect->setTexture(m_texture);
	m_spriteEffect->setParam(Matrix::Identity.Transpose(), Matrix::Identity.Transpose(), orthoMat.Transpose());

	createVertexBuffer();
	createIndexBuffer();
}

void Sprite2D::draw(const Vector2& pos, float radian, float scale, const Color& color)
{
	std::vector<SpriteVertex> vertices {
		{{-m_size.x / 2.0f, m_size.y / 2.0f,  0.0f}, {0.0f, 0.0f}, color},
		{{m_size.x / 2.0f,  m_size.y / 2.0f,  0.0f}, {1.0f, 0.0f}, color},
		{{-m_size.x / 2.0f, -m_size.y / 2.0f, 0.0f}, {0.0f, 1.0f}, color},
		{{m_size.x / 2.0f,  -m_size.y / 2.0f, 0.0f}, {1.0f, 1.0f}, color},
	};

	auto newPos = Vector2(pos.x - m_offset.x, -pos.y + m_offset.y);
	for (auto& vertex : vertices) {
		//scale * rot * trans
		const Matrix trans = Matrix::CreateScale(scale) * Matrix::CreateRotationZ(radian) * Matrix::CreateTranslation(newPos.x, newPos.y, 0.0f);
		Vector3::Transform(vertex.pos, trans, vertex.pos);
	}

	//update buffer
	D3D11_MAPPED_SUBRESOURCE resource;
	HRESULT hr = m_deviceContext->Map(m_vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	if (SUCCEEDED(hr)) {
		SpriteVertex* data = reinterpret_cast<SpriteVertex*>(resource.pData);
		memcpy(data, reinterpret_cast<void*>(vertices.data()), sizeof(SpriteVertex) * 4);
		m_deviceContext->Unmap(m_vertexBuffer.Get(), 0);
	}

	unsigned int stride = sizeof(SpriteVertex);
	unsigned int offset = 0;
	m_deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
	m_deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	m_spriteEffect->apply();

	m_deviceContext->DrawIndexed(4, 0, 0);
}

void Sprite2D::createVertexBuffer()
{
	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(SpriteVertex) * 4;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = m_device->CreateBuffer(&desc, nullptr, &m_vertexBuffer);
	if (FAILED(hr))
		throw std::runtime_error("CreateVertexBuffer Failed.");
}

void Sprite2D::createIndexBuffer()
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
