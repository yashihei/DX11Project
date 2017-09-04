#pragma once

#include "Fwd.h"
#include "Sprite.h"
#include "Actor.h"

class Particle : public Actor {
public:
	Particle(SpritePtr sprite, const Vector3& pos, const Vector3& vector, const Color& color, const float scale, bool gravity) :
		m_sprite(sprite),
		m_pos(pos), m_vector(vector), m_color(color), m_scale(scale), m_gravity(gravity)
	{}
	void update() override {
		m_pos += m_vector;
		m_vector *= 0.95f;
		m_color.w *= 0.95f;
		if (m_color.w < 0.1f)
			kill();

		if (m_gravity)
			m_vector += Vector3(0, -0.1f, 0);
	}
	void draw() override {
		m_sprite->draw(m_pos, m_color, m_scale);
	}
private:
	SpritePtr m_sprite;
	Vector3 m_pos, m_vector;
	Color m_color;
	float m_scale;
	bool m_gravity;
};

using ParticlePtr = std::shared_ptr<Particle>;
using ParticleManagerPtr = std::shared_ptr<ActorManager<Particle>>;
