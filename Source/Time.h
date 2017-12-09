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
	Time() : m_timeScale(1), m_prevTime(timeGetTime()), m_deltaTime(0) {}

	void update()
	{
		m_deltaTime = timeGetTime() - m_prevTime;
		m_prevTime = timeGetTime();
	}

	void changeScale(float scale) { m_timeScale = scale; }
	float deltaTime() const { return m_deltaTime / 1000.f * m_timeScale; }
private:
	float m_timeScale;
	DWORD m_prevTime, m_deltaTime;
};

} // namespace hks
