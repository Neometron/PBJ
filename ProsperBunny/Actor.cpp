#include "Actor.h"
#include "MathTypes.h"
using namespace HBS;
using namespace HBS_PB;
using namespace HBS_GAME;
using namespace HBS_MATH;
			
Actor::Actor(HBS::Font* font, std::vector<PB_GAME::Cell>* cells, HBS_GRAPHICS::Graphics* graphics, Landscape* ls, std::vector<HBS_MODEL::Model> models)
{
	assert(font       != nullptr);
	assert(graphics   != nullptr);
	assert(cells      != nullptr);

	this->font       = font;
	this->worldCells = cells;
	this->graphics = graphics;
	this->ls = ls;

	std::vector<Vertex> vertices;
	vertices.push_back(Vertex(-400.3f, -200.0f, 0.0f, 0.0f, 1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f));
	vertices.push_back(Vertex(-400.3f, 200.4f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f));
	vertices.push_back(Vertex(400.3f, 200.4f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f));
	vertices.push_back(Vertex(400.3f, -200.0f, 0.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f));

	std::vector<unsigned int> indices;
	indices.push_back(0); indices.push_back(1); indices.push_back(2);
	indices.push_back(2); indices.push_back(3); indices.push_back(0);

	std::vector<HBS_PB::Subset> mesh;
	mesh.push_back(HBS_PB::Subset(vertices, indices));

	model          = models.at(0);
	collisionModel = models.at(1);
	crumbs         = models.at(2);
	nameTagModel   = new HBS_MODEL::Model(graphics, mesh);

	ortho.matrix      = DirectX::XMMatrixOrthographicLH(1920, 1080, 0.1f, 100.0f);
	viewMatrix.matrix = DirectX::XMMatrixTranslation(0.0f, 0.0f, 1.0f);

	this->worldCells = cells;
	currentAction    = THINKING;
	mood             = HAPPY;
	bunnyOfInterest = nullptr;

	agef            = 0.0f;
	hunger          = 0.0f;
	dtTimeElapsed   = 0.0f;
	scalarInterval  = 0.0f;
	jumpY           = 0.0f;
	pregnantTime    = 0.0f;

	actionTimeout = 0.0f;

	posY            = 0.5;
	picked          = false;
	pregnant        = false;
	gaveBirth       = false;
	dead            = false;
	rotateDirection = 0.0f;

	pfIndex				= 0;
	guid                = 0;
}

Actor::~Actor()
{
	//delete crumbs;
	delete nameTagModel;
	//delete model;
	//delete collisionModel;
}

void Actor::initalizeActor()
{
	try
	{
		model.loadModel();
		collisionModel.loadModel();
		crumbs.loadModel();
		nameTagModel->loadModel();
	}
	catch (std::exception &p) {
		//delete model;
		//delete collisionModel;
		//delete crumbs;
		delete nameTagModel;
	}

	try
	{
		nameTagModel->mesh.at(0).textureName = "nameTag.dds";
		nameTagModel->mesh.at(0).tex = graphics->CreateImageTexture("nameTag.dds");
	}
	catch (std::exception &p) {
		delete nameTagModel;
	}
	aiComp = new AIComponent(ls);
}

void Actor::update(float dta, Camera& cam, Landscape& ls)
{
	agef += 1.0f * dta;
	if (agef > 600.0f) dead = true; //10 minutes
	if (currentAction != EATING) hunger += 1.0f * dta; //30 seconds with no food
	dtTimeElapsed  += dta;
	scalarInterval += dta;
	if (scalarInterval > 1.0f) scalarInterval = 0.0f;
	
	if (pregnant)
	{
		pregnantTime += dta;
		if (pregnantTime > 30.0f)
		{
			pregnantTime = 0.0f;
			pregnant     = false;
			gaveBirth    = true;
		}
	}

	if (currentAction == THINKING)
	{
		std::vector<PB_GAME::Cell*> cells;
		if (hunger < 30.0f)
		{
			mood = HAPPY;
			queryMap(ls.foodOnMap, PB_GAME::Occupier::FOOD, 1, 150);
		}
		else if (hunger > 30.0f && hunger < 45.0f)
		{
			mood = Mood::HUNGRY;
			queryMap(ls.foodOnMap, PB_GAME::Occupier::FOOD, 1, 450);
		}
		else if (hunger > 45.0f)
		{
			mood = Mood::STARVING;
			if (ls.foodOnMap)
			{
				PB_GAME::Cell* foodCell = ls.getClosestObjectOfDefinedType(*currCell, PB_GAME::FOOD);
				if (foodCell != nullptr)
				{
					if (searchOccupier == PB_GAME::FOOD) food = static_cast<Food*>(foodCell->getObjectFromCellMap(PB_GAME::BUNNY, guid));
					if (food != nullptr)
					{
						currentAction = INTELLIGENT_MOVE;
						searchOccupier = PB_GAME::FOOD;
					}
				}
			}
			//agef += 1.0f * dta; //double increment. Starvation causes the bunny to age faster
		}

		if (mood == HAPPY)
		{
			if (agef > 100)
			{
				int value = rand() % 10000 + 1;
				if (value > 5 && value < 1000)
				{
					mood = FRISKY;
					PB_GAME::Cell* bunnyCell = ls.getClosestObjectOfDefinedType(*currCell, PB_GAME::BUNNY);
					if (bunnyCell != nullptr)
					{
						searchOccupier = PB_GAME::BUNNY;
						bunnyOfInterest = static_cast<Actor*>(bunnyCell->getObjectFromCellMap(PB_GAME::BUNNY, guid));
						if (bunnyOfInterest != nullptr)
						{
							if (!bunnyOfInterest->pregnant && bunnyOfInterest->agef > 100 &&
								(bunnyOfInterest->currentAction != WAITING_FOR_INTERACTION && bunnyOfInterest->currentAction != PRO_CREATING))
							{
								bunnyOfInterest->currentAction = WAITING_FOR_INTERACTION;
								currentAction = INTELLIGENT_MOVE;
							}
						}
						else mood = HAPPY;
					}
					else mood = HAPPY;
				}
			}
		}
		
		if (currentAction == THINKING)
		{
			currentAction = MOVE;
			moveableCells = move();
			if (moveableCells.size() <= 0) currentAction = THINKING;
		}
		if (currentAction == MOVE || currentAction == INTELLIGENT_MOVE)
		{
			if (currentAction == INTELLIGENT_MOVE)
			{
				PB_GAME::Cell* cellOfInterest = ls.getClosestObjectOfDefinedType(*currCell, searchOccupier);
				
				if (cellOfInterest != nullptr)
				{
					cells = ls.pollSurroundingCells(*cellOfInterest);
					int cellChoice = rand() % cells.size();
					moveableCells = aiComp->moveTowardsCell(currCell, cells.at(cellChoice));

					if (moveableCells.size() < 1) currentAction = THINKING;
					else                          cellToMove = moveableCells.at(0);
					
				}
				else
				{
					moveableCells = move();
					if (moveableCells.size() <= 0) currentAction = THINKING;
					else
					{
						int cellChoice = rand() % moveableCells.size();
						cellToMove = moveableCells.at(cellChoice);
						currentAction = MOVE;
					}
				}
			}
			else
			{
				int cellChoice = rand() % moveableCells.size();
				cellToMove     = moveableCells.at(cellChoice);
			}
			
			pfIndex    = 1;
		}
	}
	
	performAction(ls, dta);
	if (picked) cam.updateCameraLocation(posX, 10.0f, posZ - 10, 0.0f, 0.0f, 0.0f);

	if (scalarInterval >= 1.0f)
		scalarInterval = 0.0f;
}

void Actor::queryMap(bool landScapePresence, const PB_GAME::Occupier occupier, int lowRange, int highRange)
{
	int rVal = (rand() % 10000 + 1); //Heavly more unlikely, or more likely depending on how many times called, needs to change!
	if (rVal > lowRange && rVal < highRange)
	{
		if (landScapePresence)
		{
			currentAction = INTELLIGENT_MOVE;
			searchOccupier = occupier;
		}
	}
}

void Actor::setPosition(float posX, float posY, float posZ)
{
	this->posX = posX;
	this->posY = posY;
	this->posZ = posZ;
}

void Actor::setCell(PB_GAME::Cell* cell)
{
	this->currentCell = cell;
}

const std::vector<HBS_PB::Subset>& Actor::getModelSubsets() const
{
	return model.getModelSubsets();
}

const std::vector<HBS_PB::Subset>& Actor::getCollisionSubsets() const
{
	return model.getModelSubsets();
}

void Actor::performAction(Landscape& ls, float dt)
{
	
	if (currentAction == MOVE)
	{
		if (posX < cellToMove->posX)
		{
			posX += 0.025f;
			rotateDirection = 0.785f;
		}
		if (posX > cellToMove->posX)
		{
			posX -= 0.025f;
			rotateDirection = -0.785f;
		}
		if (posZ < cellToMove->posY)
		{
			posZ += 0.025f;
			rotateDirection = -1.57f;
		}
		if (posZ > cellToMove->posY)
		{
			posZ -= 0.025f;
			rotateDirection = 1.57f;
		}

		if (posX < cellToMove->posX + 0.25f && posX > cellToMove->posX - 0.25f &&
			posZ < cellToMove->posY + 0.25f && posZ > cellToMove->posY - 0.25f)
		{
			currentAction = THINKING;
			posX = cellToMove->posX;
			posZ = cellToMove->posY;
			ls.removeDataFromMap(tDesc, currCell->pIndexInWorld);

			guid  = ls.setObjectOnCell(*cellToMove, PB_GAME::BUNNY, this, tDesc);
			tDesc = ls.getWorldCells()->at(cellToMove->pIndexInWorld).getTypeDesc(guid);

			currCell->removeObjectFromMap(tDesc);
			currCell = cellToMove;
		}
	} 

	if (currentAction == INTELLIGENT_MOVE)
	{
		if (posX < cellToMove->posX)
		{
			posX += 0.025f;
			rotateDirection = 0.785f;
		}
		if (posX > cellToMove->posX)
		{
			posX -= 0.025f;
			rotateDirection = -0.785f;
		}

		if (posZ < cellToMove->posY)
		{
			posZ += 0.025f;
			rotateDirection = -1.57f;
		}
		if (posZ > cellToMove->posY)
		{
			posZ -= 0.025f;
			rotateDirection = 1.57f;
		}

		if (posX < cellToMove->posX + 0.25f && posX > cellToMove->posX - 0.25f &&
			posZ < cellToMove->posY + 0.25f && posZ > cellToMove->posY - 0.25f)
		{
			if (pfIndex < moveableCells.size())
			{
				ls.removeDataFromMap(tDesc, currCell->pIndexInWorld);
				cellToMove = moveableCells.at(pfIndex);
				if (cellToMove->occupier == PB_GAME::NO_OCCUPIER)
					cellToMove->occupier = PB_GAME::BUNNY;

				guid = ls.setObjectOnCell(*cellToMove, PB_GAME::BUNNY, this, tDesc);
				tDesc = ls.getWorldCells()->at(cellToMove->pIndexInWorld).getTypeDesc(guid);
				if (currCell->occupier == PB_GAME::BUNNY)
					currCell->occupier = PB_GAME::NO_OCCUPIER;

				currCell->removeObjectFromMap(tDesc);
				currCell = cellToMove;
				pfIndex++;
			}
			else {
				currentAction = EXAMINE_SURROUNDINGS;
				pfIndex = 0;

			}
		}
	}

	if (currentAction == EXAMINE_SURROUNDINGS)
	{
		std::vector<PB_GAME::Cell*> sCells = ls.pollSurroundingCells(*currCell);
		if (sCells.size() > 0)
		{
			for (int i = 0; i < sCells.size(); i++)
			{
				for (auto const& ent1 : sCells.at(i)->pDataMap)
				{
					if (ent1.first->occupier == searchOccupier)
					{
						switch (searchOccupier)
						{
						case PB_GAME::FOOD:
							currentAction = EATING;
							break;
						case PB_GAME::BUNNY:
							currentAction = PRO_CREATING;
							bunnyOfInterest->currentAction = PRO_CREATING;
							break;
						}
					}
				}
			}
			if (currentAction == EXAMINE_SURROUNDINGS)
				currentAction = THINKING;
		}
		else currentAction = THINKING;
	}
	
	if (currentAction == THINKING)
	{
	}

	if (currentAction == EATING)
	{
		if (scalarInterval > 0.5f && scalarInterval < 0.501f)
		{
			hunger -= 250;
			if (hunger <= 0) currentAction = THINKING;
		}
	}
	if (currentAction == PRO_CREATING)
	{
		eatTimer += dt;
		if (eatTimer > 3.5f)
		{
			food->eat();
			currentAction                  = THINKING;
			if (bunnyOfInterest != nullptr)
			{
				bunnyOfInterest->currentAction = THINKING;
				bunnyOfInterest->pregnant = true;
			}
		}
	}
	if (currentAction == WAITING_FOR_INTERACTION) 
	{
		actionTimeout += dt;
		if (actionTimeout > 10.0f)
		{
			currentAction = THINKING;
			actionTimeout = 0.0f;
		}
	}
	
	setTranslationRotation(posX, 0.3f, posZ, 0.0f, rotateDirection, 0.0f);
}

std::vector<PB_GAME::Cell*> Actor::move()
{
	
	std::vector<PB_GAME::Cell*> cells;
	float xIndex = 0, yIndex = 0;
	for (int y = 0; y < 16; y++)
	{
		for (int x = 0; x < 16; x++)
		{
			if (posX < worldCells->at((y * 16) + x).posX   + 1.0f   && posX > worldCells->at((y * 16) + x).posX - 1.0f &&
				posZ < worldCells->at((y * 16) + x).posY + 1.0f && posZ > worldCells->at((y * 16) + x).posY   - 1.0f)
			{
				if (x < 15)
					 cells.push_back(&worldCells->at((y * 16) + x + 1));
				if (x > 0)
					 cells.push_back(&worldCells->at((y * 16) + x - 1));
				if (y < 15)
					 cells.push_back(&worldCells->at((y+1) * 16) + x);
				if (y > 0)
					 cells.push_back(&worldCells->at((y-1) * 16) + x);
				if (y > 0 && x > 0)
					 cells.push_back(&worldCells->at((y-1) * 16) + x - 1);
				if (y < 15 && x > 0)
					 cells.push_back(&worldCells->at((y+1) * 16) + x - 1);
				if (y > 0 && x < 15)
					 cells.push_back(&worldCells->at((y-1) * 16) + x + 1);
				if (x < 15 && y < 15)
					 cells.push_back(&worldCells->at((y+1) * 16) + x + 1);

				xIndex = x;
				yIndex = y;
			}
		}
	}
	return cells;
}

void Actor::setTranslationRotation(float posX, float posY, float posZ, float rotX, float rotY, float rotZ)
{
	model.worldMat          = DirectX::XMMatrixRotationRollPitchYaw(rotX, rotY, rotZ) * DirectX::XMMatrixTranslation(posX, posY, posZ);
	collisionModel.worldMat = DirectX::XMMatrixRotationRollPitchYaw(rotX, rotY, rotZ) * DirectX::XMMatrixTranslation(posX, posY, posZ);
}

void Actor::renderModel(Camera& cam)
{
	model.renderModel(cam);
	if (currentAction == EATING)
	{
		jumpY += 0.055f;
		if (jumpY >= 1.5f) jumpY = 0.5f;
		crumbs.worldMat = XMMatrixTranslation(currCell->posX, jumpY, currCell->posY);
		crumbs.renderModel(cam);
	}
	else if (currentAction == HBS_GAME::Actor::PRO_CREATING)
	{
		if (scaleDecrease <= 0.0f)
		{
			scaleDecrease = 1.0f;
			yRotate = 0.0f;
			tempIncrease = 0.0f;
		}
		yRotate += 0.0205f;
		tempIncrease += 0.0085f;
		heart->worldMat = DirectX::XMMatrixScaling(scaleDecrease, scaleDecrease, scaleDecrease) * DirectX::XMMatrixRotationRollPitchYaw(0.0f, yRotate, 0.0f) * DirectX::XMMatrixTranslation(posX, 2.0f + tempIncrease,posZ);
		heart->renderModel(cam);
		scaleDecrease -= 0.0045f;
	}
	if (picked)
	{
		DirectX::XMMATRIX vp = ortho.matrix * viewMatrix.matrix;
		vp = XMMatrixTranspose(vp);
		DirectX::XMMATRIX worldMat = XMMatrixTranslation(0.0f, -350.0f, 0.0f);

		graphics->ndcRender = true;
		graphics->Draw(Matrix(vp), nameTagModel->mesh, worldMat, nameTagModel->vertexBuffer, nameTagModel->indexBuffer);
		font->drawText("bunny", 0, 10);
		
		switch (mood)
		{
		case HUNGRY:
			font->drawText("hungry", 0, -60);
			break;
		case HAPPY:
			font->drawText("happy", 0, -60);
			break;
		case STARVING:
			font->drawText("starving", 0, -60);
		}
		graphics->ndcRender = false;
	}
}