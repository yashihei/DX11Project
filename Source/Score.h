// -----------------------------------------------
// File : Score.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#include "PtrAlias.h"
#include "Actor.h"
#include "MathAlias.h"
#include <d3d11.h>
#include <DirectXTK/SimpleMath.h>

namespace sp4rk {

class App;
using namespace hks;

class Score : public Actor {
public:
	explicit Score(App* app);
	void update() override;
	void draw() override;
	void addScore(int score) { m_score += score; }
	int getScore() const { return m_score; }
private:
	App* m_app;
	SpriteBatchPtr m_fontCanvas;
	int m_score, m_viewScore;
};

using ScorePtr = std::shared_ptr<Score>;

} // namespace sp4rk
