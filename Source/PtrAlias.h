// -----------------------------------------------
// File : PtrAlias.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#include <memory>
#include "Fwd.h"

using WindowPtr = std::shared_ptr<Window>;
using GraphicsPtr = std::shared_ptr<Graphics>;
using ScenePtr = std::shared_ptr<Scene>;
using CameraPtr = std::shared_ptr<Camera>;
using LightParamPtr = std::shared_ptr<LightParam>;
using InputManagerPtr = std::shared_ptr<InputManager>;
using AudioManagerPtr = std::shared_ptr<AudioManager>;
using AssetsMangerPtr = std::shared_ptr<AssetsManager>;
using FPSManagerPtr = std::shared_ptr<FPSManager>;

using ModelPtr = std::shared_ptr<Model>;
using SpritePtr = std::shared_ptr<Sprite>;
using Sprite2DPtr = std::shared_ptr<Sprite2D>;
using BasicEffectPtr = std::shared_ptr<BasicEffect>;
using SpriteEffectPtr = std::shared_ptr<SpriteEffect>;
using ToonEffectPtr = std::shared_ptr<ToonEffect>;

using CommonStatesPtr = std::shared_ptr<DirectX::CommonStates>;
using SpriteFontPtr = std::shared_ptr<DirectX::SpriteFont>;
using SpriteBatchPtr = std::shared_ptr<DirectX::SpriteBatch>;
