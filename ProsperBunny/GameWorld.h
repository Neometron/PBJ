#pragma once
#include "Input.h"
#include "Model.h"
#include "Actor.h"
#include <vector>
#include "WorldTypes.h"
#include "Window.h"
#include "Camera.h"
#include "Object.h"
#include "Landscape.h"
#include "Food.h"
#include "Font.h"
#include "GraphicsTypes.h"

namespace HBS_PB
{
	namespace HBS_PBWORLD
	{
		class GameWorld
		{
		public:
			GameWorld(const HBS_WINDOW::Handle&, HBS_PB::Camera&, HBS_GRAPHICS::Graphics*, const char*);
			~GameWorld();
		public:
			void initalizeWorld();
			void updateWorld(float, const HBS_INPUT::Input&, bool, bool);
			void renderWorld(Camera&);
		private:
			int                                pIndex;
			int                                credit;
			float                              dtDiff;
			Landscape                         *landScape;
			HBS_PB::DirectionalLight           dirLight;

			const HBS_WINDOW::Handle&          handle;
			HBS_PB::Camera&                    camera;
			HBS_GRAPHICS::Graphics		      *graphics;

			HBS::Font*						  font;
			std::vector<HBS_PB::HBS_GAME::Actor*> bunnies;
			std::vector<HBS::Food*>           food;
			std::vector<HBS_MODEL::Model>     models;
			HBS_MODEL::Model*				  timeClock;
			HBS_MODEL::Model*                 heart;

			//temp
			DirectX::XMMATRIX                 scaleMatrix;
			float                             tempIncrease;
			float                             yRotate;
			float                             scaleDecrease;
		};
	};
};
