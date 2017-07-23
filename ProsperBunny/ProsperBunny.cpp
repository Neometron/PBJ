#include "ProsperBunny.h"
#include "GameWorld.h"
#include "Graphics.h"
#include "Camera.h"
#include "Model.h"
#include "Timer.h"
#include <Windows.h>
using namespace HBS;
using namespace HBS_INPUT;
using namespace HBS_WINDOW;
using namespace HBS_GRAPHICS;
using namespace HBS_PB;

ProsperBunny::ProsperBunny(const Window& win, const Input& input)
{
	this->window = win;
	this->input = input;
}

void ProsperBunny::pbGameLoop()
{

	Timer    t;
	Graphics graphics(window.getWindowHandle(), 1920, 1080);
	Camera *cam = new Camera(graphics, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	HBS_PB::HBS_PBWORLD::GameWorld* gameWorld = new HBS_PB::HBS_PBWORLD::GameWorld(window.getWindowHandle(), *cam, &graphics, "PB3D_LandscapeModel.obj");
	gameWorld->initalizeWorld();

	POINT p;

	bool mouseDown = false;
	bool keyDown = false;
	Input input;
	bool gameRun = true;
	t.startCounter();
	while (gameRun)
	{
		float dt = t.getCounter();
		std::vector<Message> messages;
		Message msg;
		ZeroMemory(&msg, sizeof(MSG));

		mouseDown = false;
		msg = window.pollWinMessages();
		while (msg.message.message != 0)
		{
			if (msg.message.message == WM_QUIT)
			{
				gameRun = false;
				break;
			}
			if (msg.message.message == WM_LBUTTONDOWN)
			{
				keyDown   = false;
				mouseDown = true;
			}
			if (msg.message.message == WM_KEYDOWN)
			{
				mouseDown = false;
				keyDown   = true;
			}
			msg = window.pollWinMessages();
		}

		graphics.ClearBackbuffer();
		gameWorld->updateWorld(dt, input, mouseDown, keyDown);
		gameWorld->renderWorld(*cam);
		graphics.PresentBackbuffer();
	}

	delete gameWorld;
	delete cam;
	graphics.~Graphics();
}