#ifndef  WINDOW_H_
#define WINDOW_H_

#include <vector>
#ifdef WIN32
#include <Windows.h>
#endif

namespace HBS_WINDOW
{

	struct Handle { HWND hWnd; };
	struct Instance 
	{ 
#ifdef WIN32
		HINSTANCE hInstance;
		Instance(HINSTANCE hInstance) : hInstance(hInstance) {}
#endif
		Instance() {};
	};
	struct Message { MSG message; };
	class Window
	{
	public:
		Window(bool, int, int);
		Window() {};
		~Window() {};
	public:
		//Returns handle to window. If fails handleType will be nullptr
		const Handle&  createWindow(const Instance&);
		const Handle&  getWindowHandle() const;
		const Message& pollWinMessages() const;

		const int     getWindowHeight()              const;
		const int     getWindowWidth()               const;
	private:
		Handle                handle;
		Instance              instance;
		int                   width, height;
	};
};

#endif