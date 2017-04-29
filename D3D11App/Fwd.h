#pragma once

//#include <d3d11.h>
//#include <wrl/client.h>
//using Microsoft::WRL::ComPtr;
//comptr
//using DevicePtr = ComPtr<ID3D11Device>;
//using DeviceContextPtr = ComPtr<ID3D11DeviceContext>;
//using VertexShaderPtr = ComPtr<ID3D11VertexShader>;
//using PixelShaderPtr = ComPtr<ID3D11PixelShader>;
//using BufferPtr = ComPtr<ID3D11Buffer>;
//using InputLayoutPtr = ComPtr<ID3D11InputLayout>;
//using SamplerStatePtr = ComPtr<ID3D11SamplerState>;
//using ShaderResourceViewPtr = ComPtr<ID3D11ShaderResourceView>;

#include <memory>

class Effect;
class Model;
class Graphics;
class Scene;
class Camera;
class InputManager;

using EffectPtr = std::shared_ptr<Effect>;
using ModelPtr = std::shared_ptr<Model>;
using GraphicsPtr = std::shared_ptr<Graphics>;
using ScenePtr = std::shared_ptr<Scene>;
using CameraPtr = std::shared_ptr<Camera>;
using InputManagerPtr = std::shared_ptr<InputManager>;
