// -----------------------------------------------
// File : Particle.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#include "Actor.h"
#include "MathAlias.h"
#include <d3d11.h>
#include <DirectXTK/SimpleMath.h>

namespace sp4rk {

class App;
using namespace hks;

class Particle : public Actor {
public:
	explicit Particle(App* app);
	void update() override;
protected:
	App* m_app;
	float m_time;
};

using ParticleManagerPtr = std::shared_ptr<ActorManager<Particle>>;

class NormalParticle : public Particle {
public:
	NormalParticle(App* app, const Vector3& pos, const Vector3& vector, const Color& color, const float size, const float lifeTime);
	void update() override;
	void draw() override;
private:
	Vector3 m_pos, m_vector;
	Color m_color;
	float m_size, m_startSize, m_lifeTime;
};

class SpawnRing : public Particle {
public:
	SpawnRing(App* app, const Vector3& pos, const Color& color, float lifeTime);
	void update() override;
	void draw() override;
private:
	Vector3 m_pos;
	Color m_color;
	float m_size, m_lifeTime;
};

} // namespace sp4rk
