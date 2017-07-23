#pragma once
#include <vector>
#include "Landscape.h"

namespace HBS_PB
{
	namespace HBS_GAME
	{
		class AIComponent
		{
		public:
			AIComponent(Landscape*);
			~AIComponent();
		private:
			AIComponent    (const AIComponent&) {};
			void operator= (const AIComponent&) {};
		public:
			std::vector<PB_GAME::Cell*> move           (float posX, float posZ);
			std::vector<PB_GAME::Cell*> moveTowardsCell(PB_GAME::Cell*, PB_GAME::Cell*);
		private:
			Landscape     *ls;
		};
	};
};