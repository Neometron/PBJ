#include "Object.h"
using namespace HBS_PB;


Object::Object(HBS_GRAPHICS::Graphics* graphics, const char* modelName)
{
	this->graphics = graphics;
	model = new HBS_MODEL::Model(graphics, modelName);
}


void Object::setTranslationRotation(float posX, float posY, float posZ, float rotX, float rotY, float rotZ)
{
	model->worldMat = DirectX::XMMatrixRotationRollPitchYaw(rotX, rotY, rotZ) * DirectX::XMMatrixTranslation(posX, posY, posZ);
}