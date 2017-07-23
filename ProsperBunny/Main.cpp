#include "Window.h"
#include "Input.h"
#include "ProsperBunny.h"
using namespace HBS_WINDOW;
using namespace HBS_INPUT;
using namespace HBS_PB;

#ifdef WIN32
#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Window window(false, 1920, 1080);
	Handle hand = window.createWindow(Instance(hInstance));
	if (hand.hWnd == NULL) return -1;
	Input input;

	ProsperBunny pb(window, input);
	pb.pbGameLoop();

	return 0;
}

#endif