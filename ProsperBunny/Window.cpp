#include <Windows.h>
#include "Window.h"
#define WIN32_LEAN_AND_MEAN

using namespace HBS_WINDOW;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

Window::Window(bool isFullScreen, int width, int height)
{
	this->width  = width;
	this->height = height;
}

const Handle& Window::createWindow(const Instance& instance)
{
	WNDCLASSEX win;
	ZeroMemory(&win, sizeof(WNDCLASSEX));
	win.cbSize = sizeof(WNDCLASSEX);
	win.hInstance = instance.hInstance;
	win.style = CS_HREDRAW | CS_VREDRAW;
	win.hbrBackground = (HBRUSH)0;
	win.lpfnWndProc = WndProc;
	win.lpszClassName = L"PBWindow";

	if (RegisterClassEx(&win))
	{
		handle.hWnd = CreateWindowEx(NULL, L"PBWindow", L"Prosper Bunny", WS_OVERLAPPEDWINDOW, 0, 0, width, height, NULL, NULL, instance.hInstance, NULL);
		if (handle.hWnd != NULL)
		{
			ShowWindow(handle.hWnd, 10);
			return handle;
		}
		else
		{
			handle.hWnd = nullptr;
			return handle;
		}
	}
	else
	{
		handle.hWnd = nullptr;
		return handle;
	}
}

const Handle& Window::getWindowHandle() const
{
	return handle;
}

const Message& Window::pollWinMessages() const
{
	MSG                  currMSG;
	static int           activeMessages = 0;

	if (PeekMessage(&currMSG, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&currMSG);
		DispatchMessage(&currMSG);
		
		Message message;
		message.message = currMSG;
		return message;
	}
	
	//Return empty message to signify nothing to process
	Message message;
	message.message.message = 0;
	return message;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}