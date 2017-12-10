// -----------------------------------------------
// File : Timer.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#pragma comment(lib, "winmm.lib")
#include <synchapi.h>
#include <timeapi.h>

namespace hks {
	
class Time {
public:
	Time() : m_timeScale(1), m_prevTime(timeGetTime()), m_deltaTime(0), m_totalTime(0) {}

	void update()
	{
		m_deltaTime = (timeGetTime() - m_prevTime) * m_timeScale / 1000.f;
		m_totalTime += m_deltaTime;
		m_prevTime = timeGetTime();
	}

	void changeScale(float scale) { m_timeScale = scale; }
	float deltaTime() const { return m_deltaTime; }
	float totalTime() const { return m_totalTime; }
private:
	float m_timeScale;
	DWORD m_prevTime;
	float m_deltaTime, m_totalTime;
};

} // namespace hks
