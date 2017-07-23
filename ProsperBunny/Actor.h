#pragma once
#include "Camera.h"
#include "Model.h"
#include "Font.h"
#include "Landscape.h"
#include "Food.h"
#include "AIComponent.h"

namespace HBS_PB
{
	namespace HBS_GAME
	{
		class Actor
		{
		public:
			Actor (HBS::Font* font, std::vector<PB_GAME::Cell>* cells, HBS_GRAPHICS::Graphics*, Landscape*, std::vector<HBS_MODEL::Model>);
			~Actor();
		public:
			void initalizeActor();
			void setTranslationRotation(float posX, float posY, float posZ,
										float rotX, float rotY, float rotZ);

			void			  setPosition(float posX, float posY, float posZ);
			void			  setCell(PB_GAME::Cell* cell);

			const std::vector<HBS_PB::Subset>& getModelSubsets()     const;
			const std::vector<HBS_PB::Subset>& getCollisionSubsets() const;
			void              update(float dt, Camera&, Landscape& ls);
			void              renderModel(Camera& cam);
			PB_GAME::Cell*    currCell;
		public:
			bool picked;
			bool pregnant;
			bool dead;

			int               cellId;
			float             happiness;
			float             hunger;
			float                       posX, posY, posZ;
		public:
			enum Actions { MOVE, EAT, THINKING, EATING, PRO_CREATING, EXAMINE_SURROUNDINGS, INTELLIGENT_MOVE, WAITING_FOR_INTERACTION } currentAction;
			std::vector<PB_GAME::Cell*> move();
			PB_GAME::Cell*    cellToMove;
			AIComponent      *aiComp;
			void                        performAction(Landscape& ls, float);
			Actor                       *bunnyOfInterest;
			PB_GAME::Occupier           searchOccupier;
			Matrix                      viewMatrix, ortho, worldMat;
			std::vector<PB_GAME::Cell*> moveableCells;
			PB_GAME::Cell::TypeDescriptor* tDesc;
			//PB_GAME::Cell::TypeDescriptor** pTDesc;

			HBS_MODEL::Model             model;
			HBS_MODEL::Model             crumbs;
			HBS_MODEL::Model             collisionModel;
			HBS_MODEL::Model*            nameTagModel;
			HBS_MODEL::Model*                 heart;
			HBS::Food* food;
		
			//std::vector<IAction*>       actionList;
			std::vector<PB_GAME::Cell>* worldCells;
			PB_GAME::Cell*              currentCell; 
			HBS_GRAPHICS::Graphics*     graphics;

			int				            guid;
			bool                        gaveBirth;
		private:
			void queryMap(bool, const PB_GAME::Occupier, int lowRange, int highRange);
			enum Mood { HAPPY, DEPRESSED, HUNGRY, STARVING, ANGRY, SCARED, FRISKY } mood;
			enum Age { YOUNG, MIDDLE_AGED, OLD } age;
			enum Health { HEALTHY, GOOD, OK, CRITICAL } health;
			enum Ailments { EXPLODINGBUNNYSYNDROM, VAMPIRISM, BLIND, INSATIABLEHUNGER } ailment;

			//AIComponent*      aiComp;
			Landscape*        ls;
			DirectX::XMMATRIX getWorldMatrix() { return model.getWorldMatrix(); }
			HBS::Food**       foodCurrentlyEating;


			//temp
			DirectX::XMMATRIX                 scaleMatrix;
			float                             tempIncrease;
			float                             yRotate;
			float                             scaleDecrease;
			
			HBS::Font*        font;
			float             dtTimeElapsed;
			float             jumpY;
			float             scalarInterval;
			float             agef;
			float             pregnantTime;
			float             actionTimeout;
			int               pfIndex;
			float             rotateDirection;


			float eatTimer;
			float loveTimer;
			float deathTimer;
		};
	};
};

