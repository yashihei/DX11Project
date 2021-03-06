// -----------------------------------------------
// File : Scene.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

namespace hks {

class Scene {
public:
	Scene() = default;
	virtual ~Scene() = default;
	virtual Scene* update() = 0;
	virtual void draw() = 0;
};

} // namespace hks
