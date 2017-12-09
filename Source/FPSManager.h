// -----------------------------------------------
// File : FPSManager.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#pragma comment(lib, "winmm.lib")
#include <synchapi.h>
#include <timeapi.h>
#include <deque>
#include <numeric>

namespace hks {

class FPSManager {
public:
	FPSManager() : m_nowFPS(0.0) {
		m_prevTime = timeGetTime();
	}

	void sync(const int fps = 60) {
		timeBeginPeriod(1);

		const DWORD interval = (1000 / fps);
		while (timeGetTime() - m_prevTime < interval) {
			Sleep(1);
		}

		const DWORD nowTime = timeGetTime();
		const DWORD passTime = nowTime - m_prevTime;

		if (passTime != 0) {
			m_sample.push_back(1000.0 / passTime);
		}

		if (m_sample.size() >= 30) {
			m_nowFPS = std::accumulate(m_sample.begin(), m_sample.end(), 0.0) / m_sample.size();
			m_sample.clear();
		}

		m_prevTime = nowTime;

		timeEndPeriod(1);
	}

	double getFps() const { return m_nowFPS; }
private:
	double m_nowFPS;
	std::deque<double> m_sample;
	DWORD m_prevTime;
};

} // namespace hks
