#pragma once
#include <map>
#include <functional>

namespace PB_GAME
{
	enum Occupier { NO_OCCUPIER, BUNNY, FOOD };
	struct Cell
	{
		struct TypeDescriptor
		{
			unsigned int ID;
			Occupier     occupier;
		};
		float                           posX, posY, posZ;
		std::map<TypeDescriptor*, void*> pDataMap;
		//std::vector<void*>    pData;

		//These variables, and cell pointer will be used for pathfinding purposes
		int      x, y, fCost, gCost, hCost, pIndexInWorld;
		Cell*    pCellParent;
		Occupier occupier;

		TypeDescriptor* getTypeDesc(unsigned int guid)
		{
			for (auto const& ent1 : pDataMap)
			{
				if (ent1.first->ID == guid) return ent1.first;
			}
		}
		TypeDescriptor* placeObjectOnMap(void* pData, Occupier occupier, int id)
		{
			TypeDescriptor *tDesc = new TypeDescriptor();
			tDesc->ID = id;
			tDesc->occupier = occupier;
			pDataMap[tDesc] = pData;

			return tDesc;
		}
		void  removeObjectFromMap(TypeDescriptor* tDesc) { pDataMap.erase(tDesc); }
		void* getObjectFromCellMap(Occupier occupier, int id)
		{
			for (auto const& ent1 : pDataMap)
			{
				if (ent1.first->occupier == occupier && ent1.first->ID != id)
				{
					return ent1.second;
				}
			}
		}

		bool operator==(const Cell& op)
		{
			if ((int)this->posX == (int)op.posX && (int)this->posY == (int)op.posY &&
				(int)this->posZ == (int)op.posZ) return true;
			else return false;
		}
	};
}