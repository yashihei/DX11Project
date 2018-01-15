// -----------------------------------------------
// File : Score.cpp
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#include "App.h"
#include "Score.h"
#include "AssetsManager.h"
#include "Graphics.h"
#include <DirectXTK/SpriteBatch.h>
#include <DirectXTK/SpriteFont.h>
#include <string>

namespace sp4rk {

Score::Score(): m_score(0), m_viewScore(0)
{
}

void Score::update()
{
	if (m_viewScore < m_score)
		m_viewScore += (m_score - m_viewScore) / 10 + 1;
}

} // namespace sp4rk
