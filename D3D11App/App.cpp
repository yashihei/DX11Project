#include "App.h"

App::App() :
	m_hWnd(NULL), m_hInstance(GetModuleHandle(NULL)),
	m_appName("TestApp"), m_ScreenWidth(800), m_ScreenHeight(600)
{
	m_graphics = std::make_shared<Graphics>();
}

App::~App() {}

void App::run()
{
	const int answer = MessageBox(NULL, TEXT("フルスクリーンで起動しますか？"), m_appName.c_str(), MB_YESNO | MB_ICONQUESTION);
	const float fullScreen = (answer == IDYES) ? true : false;

	if (!registerWndClass())
		return;
	if (!initWindow(fullScreen))
		return;

	m_graphics->initialize(m_ScreenWidth, m_ScreenHeight, m_hWnd, fullScreen);

	MSG msg = {};

	try {
		while (msg.message != WM_QUIT) {
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			} else {
				//do some thing...
				m_graphics->beginScene();
				m_graphics->endScene();
			}
		}
	} catch (const std::runtime_error& error) {
		MessageBox(NULL, error.what(), "Runtime error", MB_OK | MB_ICONERROR);
	} catch (...) {
		MessageBox(NULL, TEXT("Unknown error"), "Error", MB_OK | MB_ICONERROR);
	}
}

bool App::registerWndClass()
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(m_hInstance, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_appName.c_str();
	wc.hIconSm = LoadIcon(m_hInstance, IDI_APPLICATION);

	if (RegisterClassEx(&wc) == 0)
		return false;

	return true;
}

bool App::initWindow(bool fullScreen)
{
	DWORD style;
	RECT rc = { 0, 0, m_ScreenWidth, m_ScreenHeight };

	if (fullScreen) {
		style = WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP;
	} else {
		style = WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_THICKFRAME);
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	}

	m_hWnd = CreateWindow(
		m_appName.c_str(),
		m_appName.c_str(),
		style,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		NULL,
		NULL,
		m_hInstance,
		NULL
	);

	if (!m_hWnd)
		return false;

	ShowWindow(m_hWnd, SW_SHOWNORMAL);
	//ShowCursor(false);

	return true;
}

LRESULT CALLBACK App::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}
