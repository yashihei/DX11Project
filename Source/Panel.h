// -----------------------------------------------
// File : Panel.h
// Copyright (c) 2018 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once
#include "PtrAlias.h"
#include "Types.h"
#include <deque>

namespace sp4rk {

class App;
using namespace hks;

class Panel {
public:
	Panel(App* app);
	void draw();
	void setScore(int score) { m_score = score; };
	void setLife(int life) { m_life = life; }
	void addLog(std::wstring log);
	void showResult() { m_result = true; }
	void hideResult() { m_result = false; }
	void showPause() { m_pause = true; }
	void hidePause() { m_pause = false; }
private:
	App* m_app;
	SpriteBatchPtr m_fontCanvas;
	int32 m_score;
	int32 m_life;
	bool m_result;
	bool m_pause;
	std::deque<std::wstring> m_logs;
};

using PanelPtr = std::shared_ptr<Panel>;

}
