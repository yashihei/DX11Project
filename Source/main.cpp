// -----------------------------------------------
// File : main.cpp
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#include "App.h"

#include <Windows.h>
#include <memory>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	auto app = std::make_unique<sp4rk::App>();

	try {
		app->run();
	} catch (const std::exception& error) {
		MessageBox(NULL, error.what(), "Exception", MB_OK | MB_ICONERROR);
	}

	return 0;
}
