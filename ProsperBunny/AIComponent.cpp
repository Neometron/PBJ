#include "AIComponent.h"
using namespace HBS_PB;
using namespace HBS_GAME;

AIComponent::AIComponent(Landscape *ls)
{
	assert(ls != nullptr);
	this->ls       = ls;
}

AIComponent::~AIComponent()
{
}

std::vector<PB_GAME::Cell*> AIComponent::moveTowardsCell(PB_GAME::Cell* currCell, PB_GAME::Cell *cell)
{
	std::vector<PB_GAME::Cell*> pathToTarget;
	if (currCell->posX < cell->posX + 0.1f && currCell->posX > cell->posX - 0.1f &&
		currCell->posY < cell->posY + 0.1f && currCell->posY > cell->posY - 0.1f)
	{
		pathToTarget.push_back(cell);
		return pathToTarget;
	}

	int xIndices[] = { 1, -1, 0, 0, -1, -1, 1, 1 };
	int yIndices[] = { 0, 0,  1,-1, -1,  1,-1, 1 };

	bool calcComplete = false;
	bool noPath = false;
	int   fCost = 0, hurestic = 0, x = 0, y = 0;
	std::vector<PB_GAME::Cell*> openList;
	std::vector<PB_GAME::Cell*> closedList;

	x = currCell->x;
	y = currCell->y;
	
	openList.push_back(&ls->getWorldCells()->at((y * 16) + x));
	openList.at(0)->fCost = hurestic = (((y * 16) + x) - ls->getObjectIndexOfCell(cell)) * 10;

	int clIndex = -1;
	while (!calcComplete)
	{
		int index = 0;
		int lowestFCost = INT_MAX;
		for (int i = 0; i < openList.size(); i++)
		{
			fCost = openList.at(i)->fCost;
			if (fCost < lowestFCost)
			{
				index = i;
				lowestFCost = fCost;
			}
		}

		closedList.push_back(openList.at(index));
		openList.erase(openList.begin() + index);
		x = closedList.at(closedList.size() - 1)->x;
		y = closedList.at(closedList.size() - 1)->y;

		clIndex++;


		for (int i = 0; i < 8; i++)
		{
			if (ls->checkIfWithinBounds(x + xIndices[i], y + yIndices[i]))
			{
				int gCost = 10;
				if (xIndices[i] != 0 && yIndices[i] != 0) gCost = 14;

				bool noRun = false;
				for (int j = 0; j < closedList.size(); j++)
					if (ls->getWorldCells()->at(((((y + yIndices[i]) * 16)) + x + xIndices[i])) == *closedList.at(j))
					{
						noRun = true;
						break;
					}

				for (int k = 0; k < openList.size(); k++)
				{
					PB_GAME::Cell celTT = ls->getWorldCells()->at(((((y + yIndices[i]) * 16)) + x + xIndices[i]));
					if (celTT == *openList.at(k))
					{
						if (openList.at(k)->gCost > gCost)
						{
							openList.at(k)->gCost = gCost;
							openList.at(k)->hCost = 10 * (abs(cell->x - celTT.x) + abs(cell->y - celTT.y));
							openList.at(k)->fCost = gCost + openList.at(k)->hCost;
							openList.at(k)->pCellParent = closedList.at(clIndex);
						}
						noRun = true;
					}
				}

				if (!noRun)
				{
					PB_GAME::Cell celTT = ls->getWorldCells()->at(((((y + yIndices[i]) * 16) + x + xIndices[i])));
					openList.push_back(&ls->getWorldCells()->at(((((y + yIndices[i]) * 16) + x + xIndices[i]))));
					openList.at(openList.size() - 1)->gCost = gCost;
					openList.at(openList.size() - 1)->hCost = 10 * (abs(cell->x - celTT.x) + abs(cell->y - celTT.y));
					openList.at(openList.size() - 1)->fCost = openList.at(openList.size() - 1)->gCost + openList.at(openList.size() - 1)->hCost;
					openList.at(openList.size() - 1)->pCellParent = closedList.at(clIndex);
				}

			}
		}

		for (int i = 0; i < closedList.size(); i++)
		{
			if ((int)closedList.at(i)->posX == (int)cell->posX && closedList.at(i)->posY == (int)cell->posY &&
				(int)closedList.at(i)->posZ == (int)cell->posZ && closedList.at(i)->occupier == cell->occupier)
				calcComplete = true;
		}
		if (openList.size() == 0)
		{
			calcComplete = true;
			noPath = true;
		}
	}

	if (noPath)
		return pathToTarget;

	PB_GAME::Cell* cellc = closedList.at(closedList.size() - 1);
	std::vector<PB_GAME::Cell*> interimList;

	do
	{
		interimList.push_back(cellc);
		cellc = cellc->pCellParent;
	} while ((int)cellc->posX != (int)currCell->posX && (int)cellc->posY != (int)currCell->posY);
	for (int i = interimList.size() - 1; i > -1; i--) pathToTarget.push_back(interimList.at(i));

	return pathToTarget;
}