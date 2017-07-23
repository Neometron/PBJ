#pragma once
#include "Window.h"
#include "Input.h"

namespace HBS_PB
{
	class ProsperBunny
	{
	public:
		ProsperBunny(const HBS_WINDOW::Window&, const HBS_INPUT::Input&);
		~ProsperBunny() {};
	public:
		void pbGameLoop();
	private:
		HBS_WINDOW::Window window;
		HBS_INPUT::Input   input;
	};
};