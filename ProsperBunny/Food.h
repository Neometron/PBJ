#pragma once
#include "Object.h"
#include "Camera.h"

namespace HBS
{
	class Food : public HBS_PB::Object
	{
	public:
		Food(HBS_GRAPHICS::Graphics*, const char*);
		~Food();
	public:
		int  eat();
		void render(HBS_PB::Camera&);
		const bool foodIsDepleted() const;
	public:
		int guid;
		PB_GAME::Cell::TypeDescriptor *tDesc;
	private:
		int replenishAmount;
		bool depleted;
	};
};

