#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include "DirectXTK/SimpleMath.h"
#include "Fwd.h"

using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;

class Sprite2D {
public:
	Sprite2D(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, ComPtr<ID3D11ShaderResourceView> texture);
	void draw(const Vector2& pos, float radian = 0.0f, float scale = 1.0f, const Color& color = {1, 1, 1, 1});
private:
	void createVertexBuffer();
	void createIndexBuffer();

	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_deviceContext;
	ComPtr<ID3D11Buffer> m_vertexBuffer;
	ComPtr<ID3D11Buffer> m_indexBuffer;
	ComPtr<ID3D11ShaderResourceView> m_texture;

	Vector2 m_size;
	SpriteEffectPtr m_spriteEffect;
};
