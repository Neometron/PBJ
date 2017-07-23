#include "Landscape.h"
using namespace HBS_PB;
using namespace PB_GAME;

Landscape::Landscape(const char* modelName, HBS_GRAPHICS::Graphics* graphics, int worldDimensionsX, int worldDimensionsY)
{
	this->worldDimensionX = worldDimensionsX;
	this->worldDimensionY = worldDimensionsY;

	landScapeModel = new HBS_MODEL::Model(graphics, "PB3D_LandscapeModel.obj");
	landScapeModel->worldMat = DirectX::XMMatrixIdentity();
	landScapeModel->worldMat = DirectX::XMMatrixTranspose(landScapeModel->worldMat);
	colModel       = new HBS_MODEL::Model(graphics, "");

	worldCells     = new std::vector<PB_GAME::Cell>;
	worldCells->resize(worldDimensionsX * worldDimensionsY);
	float posX   = -worldDimensionsX, posY = -worldDimensionsY;
	int   pIndex = 0;
	for (int y = 0; y < worldDimensionsY; y++)
	{
		posX = -worldDimensionsX;
		for (int x = 0; x < worldDimensionsX; x++)
		{
			worldCells->at((y * worldDimensionsY) + x).posX          = posX;
			worldCells->at((y * worldDimensionsY) + x).posY          = posY;
			worldCells->at((y * worldDimensionsY) + x).x             = x;
			worldCells->at((y * worldDimensionsY) + x).y             = y;
			worldCells->at((y * worldDimensionsY) + x).occupier      = NO_OCCUPIER;
			worldCells->at((y * worldDimensionsY) + x).pIndexInWorld = pIndex;

			pIndex++;
			posX += 2.0f;
		}
		posY += 2.0f;
	}
	cellDataGUID         = 0;
	cellIndexPicked      = -1;
	oppositeGendersOnMap = true;
	foodOnMap            = false;
	nullCell             = nullptr;
}

Landscape::~Landscape()
{
	colModel->~Model();
	landScapeModel->~Model();
	cells.clear();
	worldCells->clear();
}

void Landscape::loadLandscapeModel()
{
	landScapeModel->loadModel();
	landScapeModel->worldMat = DirectX::XMMatrixIdentity();
	landScapeModel->worldMat = DirectX::XMMatrixTranspose(landScapeModel->worldMat);
	colModel->loadModel();
}

void Landscape::removeDataFromMap(PB_GAME::Cell::TypeDescriptor* tDesc, int indexInWorld)
{
	worldCells->at(indexInWorld).removeObjectFromMap(tDesc);
}

std::vector<PB_GAME::Cell>* Landscape::getWorldCells() const { return worldCells; }
PB_GAME::Cell* Landscape::setObjectOnCell(void* pData, float posX, float posZ, int* guid, PB_GAME::Occupier occupier, PB_GAME::Cell::TypeDescriptor* tDesc)
{
	for (int i = 0; i < worldCells->size(); i++)
	{
		if (posX < worldCells->at(i).posX + 1.0f && posX > worldCells->at(i).posX - 1.0f &&
			posZ < worldCells->at(i).posY + 1.0f && posZ > worldCells->at(i).posY - 1.0f)
		{
			tDesc = worldCells->at(i).placeObjectOnMap(pData, occupier, ++cellDataGUID);

			worldCells->at(i).pIndexInWorld  = i;
			worldCells->at(i).posX           = posX;
			worldCells->at(i).posY           = posZ;
			*guid                            = cellDataGUID;

			profileLandscapeCells();
			return &worldCells->at(i);
		}
	}

	return NULL;
}

unsigned int Landscape::setObjectOnCell(PB_GAME::Cell& cellPtr, PB_GAME::Occupier occupier, void* pData, PB_GAME::Cell::TypeDescriptor* tDesc)
{
	cellDataGUID++;
	tDesc = cellPtr.placeObjectOnMap(pData, occupier, cellDataGUID);
	return cellDataGUID;
}

triangleStructure Landscape::checkIfPicked(HBS::HBS_MATH::Vector& worldRay, HBS::HBS_MATH::Vector& worldRayDir, Camera& camera, const HBS_WINDOW::Handle& handle)
{
	triangleStructure currentDistance(0.0f, 0.0f, 0.0f, FLT_MAX);
	for (int y = 0; y < 16; y++)
	{
		for (int x = 0; x < 16; x++)
		{
			XMMATRIX worldMat = XMMatrixRotationRollPitchYaw(1.57F, 0.0f, 0.0f) * XMMatrixTranslation(worldCells->at((y * 16) + x).posX, 0.3F, worldCells->at((y * 16) + x).posY);
			triangleStructure ts = camera.pick(worldRay, worldRayDir, colModel->getModelSubsets(), HBS_PB::Matrix(worldMat));
			if (ts.distanceToHit < currentDistance.distanceToHit)
			{
				currentDistance = ts;
				cellIndexPicked = (y * 16) + x;
			}
		}
	}

	return currentDistance;
}

std::vector<PB_GAME::Cell*> Landscape::pollSurroundingCells(PB_GAME::Cell& cell) const
{
	std::vector<PB_GAME::Cell*> cells;
	if (cell.x < 15)
		cells.push_back(&worldCells->at((cell.y * 16) + cell.x + 1));
	if (cell.x > 0)
		cells.push_back(&worldCells->at((cell.y * 16) + cell.x - 1));
	if (cell.y < 15)
		cells.push_back(&worldCells->at((cell.y + 1) * 16) + cell.x);
	if (cell.y > 0)
		cells.push_back(&worldCells->at((cell.y - 1) * 16) + cell.x);
	if (cell.y > 0 && cell.x > 0)
		cells.push_back(&worldCells->at((cell.y - 1) * 16) + cell.x - 1);
	if (cell.y < 15 && cell.x > 0)
		cells.push_back(&worldCells->at((cell.y + 1) * 16) + cell.x - 1);
	if (cell.y > 0 && cell.x < 15)
		cells.push_back(&worldCells->at((cell.y - 1) * 16) + cell.x + 1);
	if (cell.x < 15 && cell.y < 15)
		cells.push_back(&worldCells->at((cell.y + 1) * 16) + cell.x + 1);

	return cells;
}

PB_GAME::Cell* Landscape::getClosestObjectOfDefinedType(PB_GAME::Cell cell, PB_GAME::Occupier occupier)
{
	cells.clear();
	for (int i = 0; i < worldCells->size(); i++)
	{
		float dX     = cell.posX - worldCells->at(i).posX;
		float dZ     = cell.posZ - worldCells->at(i).posZ;
		float length = sqrt(dX*dX + dZ*dZ);

		if (length < 4)
		{
			for (auto const& ent1 : worldCells->at(i).pDataMap) {
				if (ent1.first->occupier == occupier)
				{
					//A special test is here. Since Bunny is the only class to call this method, an occupier of "Bunny" should not return itself
					//Also, haha. There needs to be an additional check for male, or female. Procreation ideally happens between opposite genders
					//But who are we to judge, we just want more bunnies. Needs to be an additional check to determine if bunny we are grabbing is
					//not the current bunny, and not the same gender.
					if (occupier == PB_GAME::BUNNY)
					{
						if (cell.posX != worldCells->at(i).posX && cell.posY != worldCells->at(i).posY)
							cells.push_back(&worldCells->at(i));
					}
					else if (occupier == PB_GAME::FOOD)
					{
						cells.push_back(&worldCells->at(i));
					}
					else cells.push_back(&worldCells->at(i));
				}
			}
		}
	}

	int random = 0;
	if (cells.size() > 0) random = rand() % cells.size();
	else return nullCell;

	return cells.at(random);
}

bool Landscape::checkIfWithinBounds(int x, int y) const
{
	int val = ((y * 16) + x);
	if (val > 0 && val < 256) return true;
	else                      return false;
}

int Landscape::getObjectIndexOfCell(PB_GAME::Cell* cell) 
{
	float posX = -16, posY = -16;
	for (int y = 0; y < 16; y++)
	{
		posX = -16;
		for (int x = 0; x < 16; x++)
		{
			if (posX < cell->posX + 1.0f && posX > cell->posX - 1.0f &&
				posY < cell->posY + 1.0f && posY > cell->posY - 1.0f)
			    return (y * 16) + x;
		}
		posY += 2.0f;
	}
	return -1;
}

void Landscape::profileLandscapeCells()
{
	bool tFood = false, tBunny = false;
	for (int i = 0; i < worldCells->size(); i++)
	{
		for (auto const& ent1 : worldCells->at(i).pDataMap) {
			if (ent1.first->occupier == PB_GAME::FOOD)  tFood  = true;
			if (ent1.first->occupier == PB_GAME::BUNNY) tBunny = true;
		}
	}

	foodOnMap  = tFood;
	bunnyOnMap = tBunny;
}

void Landscape::update(float dt, Camera& cam)
{
}