#include "D3D11App.h"

D3D11App::D3D11App() :
	m_hWnd(NULL), m_hInstance(GetModuleHandle(NULL)),
	m_appName("D3D11App"), m_ScreenWidth(800), m_ScreenHeight(600)
{
}

D3D11App::~D3D11App() {}

void D3D11App::run()
{
	const int answer = MessageBox(NULL, TEXT("フルスクリーンで起動しますか？"), m_appName.c_str(), MB_YESNO | MB_ICONQUESTION);
	const float fullScreen = (answer == IDYES) ? true : false;

	if (!registerWndClass())
		return;
	if (!initWindow(fullScreen))
		return;

	MSG msg = {};

	try {
		while (true) {
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				if (msg.message == WM_QUIT)
					break;
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			} else {
				//do some thing...
			}
		}
	} catch (...) {
		MessageBox(NULL, TEXT("Unknown error"), "Error", MB_OK | MB_ICONERROR);
	}
}

bool D3D11App::registerWndClass()
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

bool D3D11App::initWindow(bool fullScreen) {
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
	ShowCursor(false);

	return true;
}

LRESULT CALLBACK D3D11App::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
