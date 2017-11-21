// -----------------------------------------------
// File : FPSManager.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#pragma comment(lib, "winmm.lib")
#include <synchapi.h>
#include <timeapi.h>

class FPSManager {
public:
	FPSManager() {
		m_prevTime = timeGetTime();
	}

	void sync(const int fps = 60) {
		timeBeginPeriod(1);

		const DWORD interval = (1000 / fps);
		while (timeGetTime() - m_prevTime < interval) {
			Sleep(1);
		}

		const DWORD nowTime = timeGetTime();
		m_nowFPS = 1000 / (nowTime - m_prevTime);
		m_prevTime = nowTime;

		timeEndPeriod(1);
	}

	int getFps() const { return m_nowFPS; }
private:
	int m_nowFPS;
	DWORD m_prevTime;
};
