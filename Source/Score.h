// -----------------------------------------------
// File : Score.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#include "PtrAlias.h"
#include "Actor.h"
#include "MathAlias.h"
#include "Types.h"
#include <d3d11.h>
#include <DirectXTK/SimpleMath.h>

namespace sp4rk {

class App;
using namespace hks;

class Score {
public:
	Score();
	void update();
	void addScore(int32 score) { m_score += score; }
	int getViewScore() const { return m_viewScore; }
	int getScore() const { return m_score; }
private:
	int32 m_score, m_viewScore;
};

using ScorePtr = std::shared_ptr<Score>;

} // namespace sp4rk
