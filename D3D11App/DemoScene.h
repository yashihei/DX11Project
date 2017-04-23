#pragma once

#include <d3d11.h>
#include <wrl\client.h>

#include "Scene.h"
#include "Model.h"
#include "Effect.h"
#include "ShaderRV.h"

using Microsoft::WRL::ComPtr;

class DemoScene : public Scene {
public:
	DemoScene(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
	Scene* update() override;
	void draw() override;
private:
	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_deviceContext;
	ModelPtr m_model;
	EffectPtr m_effect;
	ShaderRVPtr m_shaderRV;
};
