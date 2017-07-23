#pragma once
#include "GraphicsTypes.h"
#include "Model.h"

namespace HBS
{
	class Font
	{
	public:
		Font(HBS_GRAPHICS::Graphics*);
		~Font();
	public:
		void loadFont();
		void drawText(std::string, int x, int y);
	private:
		HBS_GRAPHICS::Graphics      *graphics;
		HBS_PB::HBS_MODEL::Model    *fQuad;
		std::vector<HBS_PB::Texture> fontTextures;
		HBS_PB::Matrix               orthographicMatrix;
		HBS_PB::Matrix				 viewMatrix;

		float posXOffset;
	};
};