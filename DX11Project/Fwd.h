// -----------------------------------------------
// File : Fwd.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#include <memory>

class BasicEffect;
class SpriteEffect;
class ToonEffect;
class Model;
class Sprite;
class Sprite2D;
class Graphics;
class Scene;
struct LightParam;
class Camera;
class InputManager;
class AudioManager;

namespace DirectX
{
	class CommonStates;
	class SpriteFont;
	class SpriteBatch;
}

using BasicEffectPtr = std::shared_ptr<BasicEffect>;
using SpriteEffectPtr = std::shared_ptr<SpriteEffect>;
using ToonEffectPtr = std::shared_ptr<ToonEffect>;
using ModelPtr = std::shared_ptr<Model>;
using SpritePtr = std::shared_ptr<Sprite>;
using Sprite2DPtr = std::shared_ptr<Sprite2D>;
using GraphicsPtr = std::shared_ptr<Graphics>;
using ScenePtr = std::shared_ptr<Scene>;
using CameraPtr = std::shared_ptr<Camera>;
using LightParamPtr = std::shared_ptr<LightParam>;
using InputManagerPtr = std::shared_ptr<InputManager>;
using AudioManagerPtr = std::shared_ptr<AudioManager>;
using CommonStatesPtr = std::shared_ptr<DirectX::CommonStates>;
using SpriteFontPtr = std::shared_ptr<DirectX::SpriteFont>;
using SpriteBatchPtr = std::shared_ptr<DirectX::SpriteBatch>;
