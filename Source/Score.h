// -----------------------------------------------
// File : Score.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#include "App.h"
#include "PtrAlias.h"
#include "Actor.h"
#include "MathAlias.h"
#include "AssetsManager.h"
#include "Graphics.h"
#include <DirectXTK/SimpleMath.h>
#include <DirectXTK/SpriteBatch.h>
#include <DirectXTK/SpriteFont.h>

class Score : public Actor {
public:
	explicit Score(App* app) : m_app(app), m_score(0), m_viewScore(0)
	{
		m_fontCanvas = std::make_shared<DirectX::SpriteBatch>(m_app->getGraphics()->getDeviceContext().Get());
	}

	void addScore(int score)
	{
		m_score += score;
	}

	void update() override
	{
		if (m_viewScore < m_score)
			m_viewScore += (m_score - m_viewScore) / 10 + 1;
	}

	void draw() override
	{
		auto const str = L"SCORE:" + std::to_wstring(m_viewScore);
		auto font = m_app->getAssetsManager()->getFont("orbitron");
		m_fontCanvas->Begin();
		font->DrawString(m_fontCanvas.get(), str.c_str(), Vector2(10, 10), DirectX::Colors::White, 0, Vector2::Zero, 0.35f);
		m_fontCanvas->End();
	}
	
	int getScore() const { return m_score; }
private:
	App* m_app;
	SpriteBatchPtr m_fontCanvas;
	int m_score, m_viewScore;
};

using ScorePtr = std::shared_ptr<Score>;
