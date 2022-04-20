#include "../../Headers/Main/Game.h"

//Entry point for the game
// Link important libraries.
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	LBD::Game application{ hInstance };
	if (!application.Initialize()) return 0;

	return application.Run();
}
