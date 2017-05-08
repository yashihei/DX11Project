#pragma once

#include <memory>

class BasicEffect;
class SpriteEffect;
class Model;
class Sprite2D;
class Graphics;
class Scene;
class Camera;
class InputManager;
class AudioManager;

using BasicEffectPtr = std::shared_ptr<BasicEffect>;
using SpriteEffectPtr = std::shared_ptr<SpriteEffect>;
using ModelPtr = std::shared_ptr<Model>;
using Sprite2DPtr = std::shared_ptr<Sprite2D>;
using GraphicsPtr = std::shared_ptr<Graphics>;
using ScenePtr = std::shared_ptr<Scene>;
using CameraPtr = std::shared_ptr<Camera>;
using InputManagerPtr = std::shared_ptr<InputManager>;
using AudioManagerPtr = std::shared_ptr<AudioManager>;
