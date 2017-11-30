// -----------------------------------------------
// File : AssetsManager.cpp
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#include "AssetsManager.h"

#include "Model.h"
#include "Sprite.h"
#include <cassert>
#include "ShaderRV.h"
#include <DirectXTK/SpriteFont.h>

void AssetsManager::loadModel(const std::string& filePath, const std::string& alias, ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, CommonStatesPtr states, CameraPtr camera, LightParamPtr light)
{
	auto model = std::make_shared<Model>(device, deviceContext, states, camera, light);
	model->createFromObj(filePath);
	m_models[alias] = model;
}

void AssetsManager::loadSprite(const std::string& filePath, const std::string& alias, ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, CameraPtr camera)
{
	auto tex = CreateShaderResourceViewFromFile(device, s2ws(filePath).c_str());
	auto sprite = std::make_shared<Sprite>(device, deviceContext, tex, camera);
	m_sprites[alias] = sprite;
}

void AssetsManager::loadFont(const std::string& filePath, const std::string& alias, ComPtr<ID3D11Device> device)
{
	auto font = std::make_shared<DirectX::SpriteFont>(device.Get(), s2ws(filePath).c_str());
	m_fonts[alias] = font;
}

ModelPtr AssetsManager::getModel(const std::string& alias)
{
	assert(m_models.count(alias) != 0);
	return m_models[alias];
}

SpritePtr AssetsManager::getSprite(const std::string& alias)
{
	assert(m_sprites.count(alias) != 0);
	return m_sprites[alias];
}

SpriteFontPtr AssetsManager::getFont(const std::string& alias)
{
	assert(m_fonts.count(alias) != 0);
	return m_fonts[alias];
}
