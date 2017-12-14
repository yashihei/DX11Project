// -----------------------------------------------
// File : TitleScene.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once
#include "Scene.h"
#include <boost/timer.hpp>
#include "PtrAlias.h"
#include <string>

namespace sp4rk {

using namespace hks;
class App;

class TitleScene : public Scene {
public:
	explicit TitleScene(App* app);
	Scene* update() override;
	void draw() override;
private:
	App* m_app;
	boost::timer m_textTimer;
	SpriteBatchPtr m_fontCanvas;
	std::string m_titleText;
};

} // namespace sp4rk
