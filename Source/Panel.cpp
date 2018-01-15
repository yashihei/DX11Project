// -----------------------------------------------
// File : Panel.cpp
// Copyright (c) 2018 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#include "Panel.h"
#include "App.h"
#include "Graphics.h"
#include "AssetsManager.h"
#include "MathAlias.h"
#include <d3d11.h>
#include <DirectXTK/SimpleMath.h>
#include <DirectXTK/SpriteBatch.h>
#include <DirectXTK/SpriteFont.h>
#include <string>

namespace sp4rk {

Panel::Panel(App* app) : m_app(app), m_score(0), m_life(0), m_pause(false), m_result(false)
{
	m_fontCanvas = std::make_shared<DirectX::SpriteBatch>(m_app->getGraphics()->getDeviceContext().Get());
}

void Panel::draw()
{
	Vector2 center;
	UINT vpCount = 1;
	D3D11_VIEWPORT vp = {};
	m_app->getGraphics()->getDeviceContext()->RSGetViewports(&vpCount, &vp);
	center.x = vp.Width / 2.0f;
	center.y = vp.Height / 2.0f;

	auto font = m_app->getAssetsManager()->getFont("orbitron");
	auto const scoreStr = L"SCORE:" + std::to_wstring(m_score);
	auto const lifeStr = L"LIFE:" + std::to_wstring(m_life);
	Vector2(font->MeasureString(L"PAUSE")) / 2.0f;
	auto const strColor = DirectX::Colors::White;

	m_fontCanvas->Begin();
	// Draw score
	font->DrawString(m_fontCanvas.get(), scoreStr.c_str(), Vector2(10, 10), strColor, 0, Vector2::Zero, 0.35f);
	// Draw life
	font->DrawString(m_fontCanvas.get(), lifeStr.c_str(), Vector2(10, 45), strColor, 0, Vector3::Zero, 0.35f);
	// Draw Pause
	if (m_pause) {
		font->DrawString(m_fontCanvas.get(), L"PAUSE", center, strColor, 0, Vector2(font->MeasureString(L"PAUSE")) / 2.f, 0.7f);
	}
	// Draw Result
	if (m_result) {
		font->DrawString(m_fontCanvas.get(), L"GAME OVER", center, strColor, 0, Vector2(font->MeasureString(L"GAME OVER")) / 2.f, 0.7f);
	}
	// Draw Log
	int cnt = 1;
	for (auto log : m_logs) {
		font->DrawString(m_fontCanvas.get(), log.c_str(), Vector2(10, vp.Height - cnt * 15 - 10), strColor * (1.f - 0.15f * cnt), 0, Vector3::Zero, 0.15f);
		cnt++;
	}
	m_fontCanvas->End();
}

void Panel::addLog(std::wstring log)
{
	m_logs.push_front(log);
	if (m_logs.size() > 5) {
		m_logs.pop_back();
	}
}

}
