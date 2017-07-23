#include "Bunny.h"
using namespace HBS_PB;
using namespace HBS_GAME;
/*
class Procreate : public Bunny::BunnyAction
{
	virtual bool canExecute()
	{
		int chanceOfProcreating = rand() % 10000 + 1;
		if (bunnyActor->currentMood == AIComponent::HAPPY && chanceOfProcreating < 250)
		{
			std::vector<PB_GAME::Cell*> sCells = bunnyActor->ls->pollSurroundingCells(*bunnyActor->actor->currCell);
			if (sCells.size() > 0)
			{
				for (int i = 0; i < sCells.size(); i++)
				{
					if (sCells.at(i)->occupier == bunnyActor->actor->searchOccupier)
					{
						switch (bunnyActor->actor->searchOccupier)
						{
						case PB_GAME::BUNNY:
							bunnyActor->currentAction = AIComponent::PRO_CREATING;
							//bunnyActor->actor->bunnyOfInterest->currentAction = AIComponent::PRO_CREATING;
							return true;
							break;
						}
					}
				}
				if (bunnyActor->currentAction == AIComponent::EXAMINE_SURROUNDINGS)
					bunnyActor->currentAction = AIComponent::THINKING;
			}
			else bunnyActor->currentAction = AIComponent::THINKING;
		}
		else bunnyActor->currentAction = AIComponent::THINKING;
		return false;
	}
	virtual void update(const float dta)
	{
		bunnyActor->scalarInterval += dta;
		if (bunnyActor->scalarInterval > 1.0f) bunnyActor->scalarInterval = 0.0f;
		if (bunnyActor->scalarInterval > 0.5f && bunnyActor->scalarInterval < 0.501f)
		{
			bunnyActor->currentAction = AIComponent::THINKING;
			if (bunnyActor->actor->bunnyOfInterest->bunnyOfInterest != nullptr)
			{
				bunnyActor->bunnyOfInterest->currentAction = AIComponent::THINKING;
				bunnyActor->bunnyOfInterest->actor->pregnant = true;
			}
		}
	}
	virtual void invoke()
	{

	}
};

class Eat : public Bunny::BunnyAction
{
	virtual bool canExecute()
	{
		//Check Hunger Level. 
		if (bunnyActor->actor->hunger < 60.0f)
		{
			bunnyActor->currentMood = AIComponent::Mood::HAPPY;
			return false;
		}
		else if (bunnyActor->actor->hunger > 60.0f && bunnyActor->actor->hunger < 100.0f)
		{
			bunnyActor->currentMood = AIComponent::Mood::HUNGRY;
			if ((rand() % 1000 + 1) == 1)
			{
				queryFoodOnMap();
			}
			else return false;
		}
		else if (bunnyActor->actor->hunger > 100.0f)
		{
			bunnyActor->currentMood = AIComponent::Mood::STARVING;
			queryFoodOnMap();
		}

		std::vector<PB_GAME::Cell*> sCells = bunnyActor->ls->pollSurroundingCells(*bunnyActor->actor->currCell);
		if (sCells.size() > 0)
		{
			for (int i = 0; i < sCells.size(); i++)
			{
				if (sCells.at(i)->occupier == bunnyActor->actor->searchOccupier)
				{
					switch (bunnyActor->actor->searchOccupier)
					{
					case PB_GAME::FOOD:
						bunnyActor->currentAction = AIComponent::EATING;
						return true;
						//bunnyActor->actor->bunnyOfInterest->currentAction = AIComponent::PRO_CREATING;
						break;
					}
				}
			}

			if (bunnyActor->currentAction == AIComponent::EXAMINE_SURROUNDINGS)
			{
				bunnyActor->currentAction = AIComponent::INTELLIGENT_MOVE;
				bunnyActor->searchOccupier = PB_GAME::FOOD;
			}
		}
		else bunnyActor->currentAction = AIComponent::THINKING;

		return false;
	}
	virtual void update(const float dta)
	{
		bunnyActor->scalarInterval += dta;
		if (bunnyActor->scalarInterval > 1.0f) bunnyActor->scalarInterval = 0.0f;
		if (bunnyActor->scalarInterval > 0.5f && bunnyActor->scalarInterval < 0.501f)
		{
			bunnyActor->actor->hunger -= 250;
			if (bunnyActor->actor->hunger <= 0) bunnyActor->currentAction = AIComponent::THINKING;
		}
	}
	virtual void invoke()
	{

	}
};

class Frisky : public Bunny::BunnyAction
{
	virtual bool canExecute()
	{
		int chanceOfProcreating = rand() % 10000 + 1;
		if (bunnyActor->currentMood == AIComponent::HAPPY && chanceOfProcreating < 450)
		{
			bunnyActor->currentMood = AIComponent::Mood::FRISKY;
			PB_GAME::Cell* bunnyCell = bunnyActor->ls->getClosestObjectOfDefinedType(*bunnyActor->actor->currCell, PB_GAME::BUNNY);
			if (bunnyCell != nullptr)
			{
				bunnyActor->actor->searchOccupier = PB_GAME::BUNNY;
				bunnyActor->bunnyOfInterest = static_cast<Bunny*>(bunnyCell->pData);
				if (bunnyActor->bunnyOfInterest != nullptr)
				{
					bunnyActor->bunnyOfInterest->currentAction = AIComponent::Actions::WAITING_FOR_INTERACTION;
					std::vector<PB_GAME::Cell*> sCells = bunnyActor->ls->pollSurroundingCells(*bunnyActor->actor->currCell);
					if (sCells.size() > 0)
					{
						for (int i = 0; i < sCells.size(); i++)
						{
							if (sCells.at(i)->occupier == bunnyActor->actor->searchOccupier)
							{
								switch (bunnyActor->actor->searchOccupier)
								{
								case PB_GAME::BUNNY:
									bunnyActor->currentAction = AIComponent::PRO_CREATING;
									//bunnyActor->actor->bunnyOfInterest->currentAction = AIComponent::PRO_CREATING;
									return true;
									break;
								}
							}
						}
						if (bunnyActor->currentAction == AIComponent::FRISKY)
						{
							bunnyActor->currentAction = AIComponent::INTELLIGENT_MOVE;
							return false;
						}
					}
					else bunnyActor->currentAction = AIComponent::THINKING;
				}
				else bunnyActor->currentMood = AIComponent::Mood::HAPPY;
			}
			else bunnyActor->currentMood = AIComponent::Mood::HAPPY;
		}
		else bunnyActor->currentAction = AIComponent::THINKING;
		return false;
	}

	virtual void update(const float)
	{

	}

	virtual void invoke()
	{

	}
};

class Move : public Bunny::BunnyAction
{
	virtual bool canExecute()
	{
		if (bunnyActor->currentAction == AIComponent::MOVE)
		{
			std::vector<PB_GAME::Cell*> cells;
			float xIndex = 0, yIndex = 0;
			for (int y = 0; y < 16; y++)
			{
				for (int x = 0; x < 16; x++)
				{
					if (bunnyActor->actor->posX < bunnyActor->actor->worldCells->at((y * 16) + x).posX + 1.0f   && bunnyActor->actor->posX > bunnyActor->actor->worldCells->at((y * 16) + x).posX - 1.0f &&
						bunnyActor->actor->posZ < bunnyActor->actor->worldCells->at((y * 16) + x).posY + 1.0f   && bunnyActor->actor->posZ > bunnyActor->actor->worldCells->at((y * 16) + x).posY - 1.0f)
					{
						if (x < 15)
							if (bunnyActor->actor->worldCells->at((y * 16) + x + 1).pData == NULL)     cells.push_back(&bunnyActor->actor->worldCells->at((y * 16) + x + 1));
						if (x > 0)
							if (bunnyActor->actor->worldCells->at((y * 16) + x - 1).pData == NULL)     cells.push_back(&bunnyActor->actor->worldCells->at((y * 16) + x - 1));
						if (y < 15)
							if (bunnyActor->actor->worldCells->at((y * 16 + 1) + x).pData == NULL)     cells.push_back(&bunnyActor->actor->worldCells->at((y + 1) * 16) + x);
						if (y > 0)
							if (bunnyActor->actor->worldCells->at((y * 16 - 1) + x).pData == NULL)     cells.push_back(&bunnyActor->actor->worldCells->at((y - 1) * 16) + x);
						if (y > 0 && x > 0)
							if (bunnyActor->actor->worldCells->at((y * 16 - 1) + x - 1).pData == NULL) cells.push_back(&bunnyActor->actor->worldCells->at((y - 1) * 16) + x - 1);
						if (y < 15 && x > 0)
							if (bunnyActor->actor->worldCells->at((y * 16 + 1) + x - 1).pData == NULL) cells.push_back(&bunnyActor->actor->worldCells->at((y + 1) * 16) + x - 1);
						if (y > 0 && x < 15)
							if (bunnyActor->actor->worldCells->at((y * 16 - 1) + x + 1).pData == NULL) cells.push_back(&bunnyActor->actor->worldCells->at((y - 1) * 16) + x + 1);
						if (x < 15 && y < 15)
							if (bunnyActor->actor->worldCells->at((y * 16 + 1) + x + 1).pData == NULL) cells.push_back(&bunnyActor->actor->worldCells->at((y + 1) * 16) + x + 1);

						xIndex = x;
						yIndex = y;
					}
				}
			}
			if (cells.size() > 0)
			{
				int cellChoice = rand() % cells.size();
				bunnyActor->actor->cellToMove = cells.at(cellChoice);
				return true;
			}
			else return false;
		}
		return false;
	}
	virtual void update(const float)
	{
		if (bunnyActor->actor->posX < bunnyActor->actor->cellToMove->posX) bunnyActor->actor->posX += 0.025f;
		if (bunnyActor->actor->posX > bunnyActor->actor->cellToMove->posX) bunnyActor->actor->posX -= 0.025f;
		if (bunnyActor->actor->posZ < bunnyActor->actor->cellToMove->posY) bunnyActor->actor->posZ += 0.025f;
		if (bunnyActor->actor->posZ > bunnyActor->actor->cellToMove->posY) bunnyActor->actor->posZ -= 0.025f;

		if (bunnyActor->actor->posX < bunnyActor->actor->cellToMove->posX + 0.25f && bunnyActor->actor->posX > bunnyActor->actor->cellToMove->posX - 0.25f &&
			bunnyActor->actor->posZ < bunnyActor->actor->cellToMove->posY + 0.25f && bunnyActor->actor->posZ > bunnyActor->actor->cellToMove->posY - 0.25f)
		{
			bunnyActor->currentAction = AIComponent::THINKING;
			bunnyActor->actor->posX = bunnyActor->actor->cellToMove->posX;
			bunnyActor->actor->posZ = bunnyActor->actor->cellToMove->posY;

			if (bunnyActor->actor->cellToMove->occupier == PB_GAME::NO_OCCUPIER)
				bunnyActor->actor->cellToMove->occupier = PB_GAME::BUNNY;

			bunnyActor->actor->cellToMove->pData = this;
			if (bunnyActor->actor->currCell->occupier == PB_GAME::BUNNY)
				bunnyActor->actor->currCell->occupier = PB_GAME::NO_OCCUPIER;
			bunnyActor->actor->currCell->pData = NULL;
			bunnyActor->actor->currCell = bunnyActor->actor->cellToMove;
		}
	}
	virtual void invoke()
	{

	}
};

class IMove : public Bunny::BunnyAction
{
	virtual bool canExecute()
	{
		if (bunnyActor->currentAction == AIComponent::INTELLIGENT_MOVE)
		{
			cells                            = bunnyActor->ls->pollSurroundingCells(*bunnyActor->ls->getClosestObjectOfDefinedType(*bunnyActor->actor->currCell, bunnyActor->actor->searchOccupier));
			int cellChoice                   = rand() % cells.size();
			bunnyActor->actor->moveableCells = bunnyActor->aiComponent->moveTowardsCell(bunnyActor->actor->currCell, cells.at(cellChoice));

			if (bunnyActor->actor->moveableCells.size() < 1) bunnyActor->currentAction  = AIComponent::THINKING;
			else                          bunnyActor->actor->cellToMove                 = bunnyActor->actor->moveableCells.at(0);

			return true;
		}
		return false;
	}
	virtual void update(const float)
	{
		if (bunnyActor->actor->posX < bunnyActor->actor->cellToMove->posX) bunnyActor->actor->posX += 0.025f;
		if (bunnyActor->actor->posX > bunnyActor->actor->cellToMove->posX) bunnyActor->actor->posX -= 0.025f;

		if (bunnyActor->actor->posZ < bunnyActor->actor->cellToMove->posY) bunnyActor->actor->posZ += 0.025f;
		if (bunnyActor->actor->posZ > bunnyActor->actor->cellToMove->posY) bunnyActor->actor->posZ -= 0.025f;

		if (bunnyActor->actor->posX < bunnyActor->actor->cellToMove->posX + 0.25f && bunnyActor->actor->posX > bunnyActor->actor->cellToMove->posX - 0.25f &&
			bunnyActor->actor->posZ < bunnyActor->actor->cellToMove->posY + 0.25f && bunnyActor->actor->posZ > bunnyActor->actor->cellToMove->posY - 0.25f)
		{
			if (pfIndex < bunnyActor->actor->moveableCells.size())
			{
				bunnyActor->actor->cellToMove = bunnyActor->actor->moveableCells.at(pfIndex);
				if (bunnyActor->actor->cellToMove->occupier == PB_GAME::NO_OCCUPIER)
					bunnyActor->actor->cellToMove->occupier = PB_GAME::BUNNY;

				bunnyActor->actor->cellToMove->pData = this;
				if (bunnyActor->actor->currCell->occupier == PB_GAME::BUNNY)
					bunnyActor->actor->currCell->occupier = PB_GAME::NO_OCCUPIER;

				bunnyActor->actor->currCell->pData = NULL;
				bunnyActor->actor->currCell = bunnyActor->actor->cellToMove;
				pfIndex++;
			}
			else {
				bunnyActor->actor->currentAction = bunnyActor->actor->EXAMINE_SURROUNDINGS;
				pfIndex = 0;
			}
		}
	}
	virtual void invoke()
	{

	}
};

class Waiting : public Bunny::BunnyAction
{
	virtual bool canExecute()
	{

	}
	virtual void update(const float)
	{

	}
	virtual void invoke()
	{

	}
};

Bunny::Bunny(HBS::Font* font, std::vector<PB_GAME::Cell>* cells, HBS_GRAPHICS::Graphics* graphics, Landscape* ls, const char* modelName)
{
	actor       = new Actor(font, cells, graphics, ls, modelName);
	aiComponent = new AIComponent(ls);

	actionList.push_back(new Procreate());
	actionList.push_back(new Eat());
	actionList.push_back(new Frisky());
	actionList.push_back(new Move());
	actionList.push_back(new IMove());
}

Bunny::~Bunny()
{
}

void Bunny::intializeBunny()
{
	try
	{
		actor->initalizeActor();
	}
	catch (std::exception e) {
		int shiiiiit = 12;
	}
}

void Bunny::update(float dt)
{
	//update actor component

	for (int i = 0; i < actionList.size(); i++)
	{
		if (actionList.at(i)->isExecuting)
		{
			actionList.at(i)->update(dt);
			break;
		}

		if (i == actionList.size() - 1)
		{
			for (int j = 0; j < actionList.size(); j++)
				if (actionList.at(j)->canExecute()) actionList.at(j)->isExecuting = true;
		}
	}
}

void Bunny::render()
{

}*/