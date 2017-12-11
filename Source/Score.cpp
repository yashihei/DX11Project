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

Score::Score(App* app): m_app(app), m_score(0), m_viewScore(0)
{
	m_fontCanvas = std::make_shared<DirectX::SpriteBatch>(m_app->getGraphics()->getDeviceContext().Get());
}

void Score::update()
{
	if (m_viewScore < m_score)
		m_viewScore += (m_score - m_viewScore) / 10 + 1;
}

void Score::draw()
{
	auto const str = L"SCORE:" + std::to_wstring(m_viewScore);
	auto font = m_app->getAssetsManager()->getFont("orbitron");
	m_fontCanvas->Begin();
	font->DrawString(m_fontCanvas.get(), str.c_str(), Vector2(10, 10), DirectX::Colors::White, 0, Vector2::Zero, 0.35f);
	m_fontCanvas->End();
}

} // namespace sp4rk
