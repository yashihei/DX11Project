#pragma once

#include <memory>

class BasicEffect;
class Model;
class Graphics;
class Scene;
class Camera;
class InputManager;
class AudioManager;

using BasicEffectPtr = std::shared_ptr<BasicEffect>;
using ModelPtr = std::shared_ptr<Model>;
using GraphicsPtr = std::shared_ptr<Graphics>;
using ScenePtr = std::shared_ptr<Scene>;
using CameraPtr = std::shared_ptr<Camera>;
using InputManagerPtr = std::shared_ptr<InputManager>;
using AudioManagerPtr = std::shared_ptr<AudioManager>;
