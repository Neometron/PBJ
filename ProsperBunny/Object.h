#pragma once
#include "Model.h"
#include "WorldTypes.h"

namespace HBS_PB
{
	class Object
	{
	public:
		Object() {};
		Object(HBS_GRAPHICS::Graphics*, const char*);
		~Object() {};
	public:
		float posX, posY, posZ;
		void setTranslationRotation(float posX, float posY, float posZ, float rotX, float rotY, float rotZ);
	public:
		PB_GAME::Cell*              cell;
		HBS_MODEL::Model*           model;
	private:
		HBS_GRAPHICS::Graphics* graphics;
	};
};