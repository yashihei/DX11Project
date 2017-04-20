#pragma once

class Scene {
public:
	Scene() = default;
	virtual ~Scene() = default;
	virtual Scene* update() = 0;
	virtual void draw() = 0;
};
