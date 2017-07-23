#include "Food.h"
using namespace HBS;
using namespace HBS_PB;

Food::Food(HBS_GRAPHICS::Graphics *graphics, const char *modelName) : HBS_PB::Object(graphics, modelName)
{
	replenishAmount = 3;
	depleted = false;
	model = new HBS_MODEL::Model(graphics, modelName);
	model->loadModel();
	guid = 0;
}

Food::~Food()
{
	delete model;
}

int Food::eat()
{
	if (!depleted)
	{
		replenishAmount--;

		if (replenishAmount <= 0) depleted = true;
		return replenishAmount;
	}
	else return 0;
}

const bool Food::foodIsDepleted() const { return (replenishAmount > 0) ? false : true; }
void Food::render(Camera& cam)
{
	model->renderModel(cam);
}