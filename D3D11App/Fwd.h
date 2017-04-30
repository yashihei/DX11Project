#pragma once

#include <memory>

class Effect;
class Model;
class Graphics;
class Scene;
class Camera;
class InputManager;
class AudioManager;

using EffectPtr = std::shared_ptr<Effect>;
using ModelPtr = std::shared_ptr<Model>;
using GraphicsPtr = std::shared_ptr<Graphics>;
using ScenePtr = std::shared_ptr<Scene>;
using CameraPtr = std::shared_ptr<Camera>;
using InputManagerPtr = std::shared_ptr<InputManager>;
using AudioManagerPtr = std::shared_ptr<AudioManager>;
