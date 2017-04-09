#include "D3D11App.h"
#include <memory>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	auto app = std::make_unique<D3D11App>();
	app->run();
	return 0;
}
