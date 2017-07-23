#pragma once
#include "Model.h"
#include "WorldTypes.h"
#include "Graphics.h"
#include "Window.h"

namespace HBS_PB
{
	class Landscape
	{
	public:
		Landscape(const char*, HBS_GRAPHICS::Graphics*, int worldDimensionsX, int worldDimensionsY);
		~Landscape();
	public:
		void                        update(float, Camera&);
		void                        loadLandscapeModel();

		void                        removeDataFromMap(PB_GAME::Cell::TypeDescriptor*, int);
		std::vector<PB_GAME::Cell>* getWorldCells() const;
		PB_GAME::Cell*              setObjectOnCell(void*, float posX, float posZ, int* guid, PB_GAME::Occupier, PB_GAME::Cell::TypeDescriptor*);
		unsigned int                setObjectOnCell(PB_GAME::Cell& cell, PB_GAME::Occupier, void*,               PB_GAME::Cell::TypeDescriptor*);
		int                         getObjectIndexOfCell(PB_GAME::Cell*);
		triangleStructure           checkIfPicked(HBS::HBS_MATH::Vector& worldRay, HBS::HBS_MATH::Vector& worldRayDir, Camera&, const HBS_WINDOW::Handle& handle);

		std::vector<PB_GAME::Cell*> pollSurroundingCells(PB_GAME::Cell&) const;
		bool                        checkIfWithinBounds(int x, int y)                                         const;
		PB_GAME::Cell*              getClosestObjectOfDefinedType(PB_GAME::Cell bunnyCell, PB_GAME::Occupier);

	public:
		std::vector<PB_GAME::Cell*>  cells;
		PB_GAME::Cell*               nullCell;
		HBS_MODEL::Model*           colModel;
		HBS_MODEL::Model*           landScapeModel;
		int                         cellIndexPicked;

		bool                        foodOnMap;
		bool                        oppositeGendersOnMap;
		bool                        bunnyOnMap;
	private:
		HBS_GRAPHICS::Graphics*     graphics;
		std::vector<PB_GAME::Cell>* worldCells;
		float                       worldDimensionX, worldDimensionY;
		int							cellDataGUID;
	private:
		void                        profileLandscapeCells();
	};
};
