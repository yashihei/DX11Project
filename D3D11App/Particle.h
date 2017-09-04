#pragma once

#include "Fwd.h"
#include "Sprite.h"
#include "Actor.h"

class Particle : public Actor {
public:
	Particle(SpritePtr sprite, const Vector3& pos, const Vector3& vector, const Color& color) :
		m_sprite(sprite),
		m_pos(pos), m_vector(vector), m_color(color), m_scale(1.0f)
	{}
	void update() override {
		m_pos += m_vector;
		m_vector *= 0.95f;
		m_color.w *= 0.95f;
		if (m_color.w < 0.1f)
			kill();
	}
	void draw() override {
		m_sprite->draw(m_pos, m_color, m_scale);
	}
private:
	SpritePtr m_sprite;
	Vector3 m_pos, m_vector;
	Color m_color;
	float m_scale;
};

using ParticlePtr = std::shared_ptr<Particle>;
using ParticleManagerPtr = std::shared_ptr<ActorManager<Particle>>;
