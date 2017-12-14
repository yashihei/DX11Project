// -----------------------------------------------
// File : AssetsManager.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#include "Fwd.h"
#include "PtrAlias.h"
#include <unordered_map>
#include <d3d11.h>
#include <wrl/client.h>

namespace hks {

using Microsoft::WRL::ComPtr;

class AssetsManager {
public:
	AssetsManager() = default;

	void loadModel(const std::string& filePath, const std::string& alias, ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, CameraPtr camera, LightParamPtr light);
	void loadSprite(const std::string& filePath, const std::string& alias, ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, CameraPtr camera);
	void loadFont(const std::string& filePath, const std::string& alias, ComPtr<ID3D11Device> device);
	ModelPtr getModel(const std::string& alias);
	SpritePtr getSprite(const std::string& alias);
	SpriteFontPtr getFont(const std::string& alias);

	void allClear()
	{
		m_models.clear();
		m_sprites.clear();
		m_fonts.clear();
	}
private:
	std::unordered_map<std::string, ModelPtr> m_models;
	std::unordered_map<std::string, SpritePtr> m_sprites;
	std::unordered_map<std::string, SpriteFontPtr> m_fonts;
};

} // namespace hks
