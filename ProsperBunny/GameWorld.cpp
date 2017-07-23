#include "GameWorld.h"
#include "Input.h"
#include <time.h>
#include "Timer.h"
using namespace HBS;
using namespace HBS_PB;
using namespace HBS_PBWORLD;
using namespace HBS_GRAPHICS;

GameWorld::GameWorld(const HBS_WINDOW::Handle& handle, HBS_PB::Camera& cam, HBS_GRAPHICS::Graphics* graphics, const char* modelPath) : camera(cam), handle(handle)
{
	this->graphics = graphics;
	
	landScape      = new Landscape("PB3D_LandscapeModel.obj", graphics, 16, 16);
	font           = new HBS::Font(graphics);

	std::vector<Vertex> vertices;
	vertices.push_back(Vertex(-150.0f, -75.0f, 0.0f, 0.0f, 1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f));
	vertices.push_back(Vertex(-150.0f, 75.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f));
	vertices.push_back(Vertex(150.0f, 75.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f));
	vertices.push_back(Vertex(150.0f, -75.0f, 0.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f));

	std::vector<unsigned int> indices;
	indices.push_back(0); indices.push_back(1);	indices.push_back(2);
	indices.push_back(2); indices.push_back(3); indices.push_back(0);

	std::vector<HBS_PB::Subset> mesh;
	mesh.push_back(HBS_PB::Subset(vertices, indices));

	timeClock                         = new HBS_MODEL::Model(graphics, mesh);
	heart                             = new HBS_MODEL::Model(graphics, "heart.obj");
	timeClock->mesh.at(0).textureName = "sunDial.dds";
	timeClock->mesh.at(0).tex         = graphics->CreateImageTexture("sunDial.dds");

	srand(time(NULL));
	//int numberOfBunnies = rand() % (12 * 12) / 8 + 1;
	int numberOfBunnies = 2;
	bunnies.resize(numberOfBunnies);

	models.push_back(HBS_MODEL::Model(graphics, "bunny.obj"));
	models.push_back(HBS_MODEL::Model(graphics, "collisionMesh.obj"));
	models.push_back(HBS_MODEL::Model(graphics, "crumbs.obj"));

	for (int i = 0; i < numberOfBunnies; i++)
	{
 		bunnies.at(i) = new HBS_GAME::Actor(font, landScape->getWorldCells(), graphics, landScape, models);

		int randomX = 0;
		int randomY = 0;
		int guid    = 0;
		bool isPlaceable = false;
		PB_GAME::Cell* cell;
		do
		{
			randomX = (rand() % 32) - 16;
			randomY = (rand() % 32) - 16;

			cell                                   = landScape->setObjectOnCell(bunnies.at(i), randomX, randomY, &guid, PB_GAME::BUNNY, bunnies.at(i)->tDesc);
			if (cell != NULL) bunnies.at(i)->tDesc = landScape->getWorldCells()->at(cell->pIndexInWorld).getTypeDesc(guid);
		} while (cell == NULL);

		bunnies.at(i)->currCell = cell;
		bunnies.at(i)->posX     = randomX; 
		bunnies.at(i)->posY     = 1.0f;
		bunnies.at(i)->posZ     = randomY;
		bunnies.at(i)->guid     = guid;

	}

	pIndex = -1;
	dtDiff = 0.0f;
	credit = 0;
}

GameWorld::~GameWorld()
{
	delete landScape;
	for (int i = 0; i < bunnies.size(); i++)
	{
		delete bunnies.at(i);
	}
	for (int i = 0; i < food.size(); i++)
	{
		delete food.at(i);
	}
	food.clear();
	bunnies.clear();

	delete font;
	delete timeClock;
	delete heart;

	graphics = nullptr;
}

void GameWorld::initalizeWorld()
{
	landScape->loadLandscapeModel();
	for (int i = 0; i < bunnies.size(); i++) bunnies.at(i)->initalizeActor();
	heart->loadModel();
	for (int i = 0; i < bunnies.size(); i++) bunnies.at(i)->heart = heart;
	timeClock->loadModel();
}

void GameWorld::updateWorld(float dt, const HBS_INPUT::Input& input, bool mouseDown, bool keyDown)
{
	static int iteration = 1;
	float      dta       = 0.0f;
	dta    = dt - dtDiff;
	dtDiff = dt;

	if (mouseDown)
	{
		POINT             p;
		triangleStructure ts;
		GetCursorPos(&p);
		ScreenToClient(handle.hWnd, &p);

		triangleStructure currentDistance(0.0f, 0.0f, 0.0f, FLT_MAX);
		HBS::HBS_MATH::Vector worldRay, worldRayDir;

		camera.pickRayVector(p.x, p.y, worldRay, worldRayDir);
		for (int i = 0; i < bunnies.size(); i++)
		{
			triangleStructure ts = bunnies.at(i)->collisionModel.checkIfPicked(worldRay, worldRayDir, camera, handle);
			if (ts.distanceToHit < FLT_MAX)
			{
				if (ts.distanceToHit < currentDistance.distanceToHit)
				{
					currentDistance = ts;
					if (pIndex != -1) bunnies.at(pIndex)->picked = false;
					pIndex           = i;
				}
			}
		}

		ts = landScape->checkIfPicked(worldRay, worldRayDir, camera, handle);
		if (ts.distanceToHit < currentDistance.distanceToHit)
		{
			currentDistance = ts;
			pIndex = landScape->cellIndexPicked;

			if (food.size() < 6)
			{
				food.resize(food.size() + 1);
				food.at(food.size() - 1) = new Food(graphics, "food.obj");

				int guid = 0;
				//food.at(food.size() - 1)->posX = landScape->getWorldCells()->at(pIndex).posX;
				//food.at(food.size() - 1)->posY = 1.0f;
				//food.at(food.size() - 1)->posZ = landScape->getWorldCells()->at(pIndex).posY;

				food.at(food.size() - 1)->setTranslationRotation(landScape->getWorldCells()->at(pIndex).posX, 1.0f, landScape->getWorldCells()->at(pIndex).posY, 0.0f, 0.0f, 0.0f);
				PB_GAME::Cell* cell = landScape->setObjectOnCell(food.at(food.size() - 1), landScape->getWorldCells()->at(pIndex).posX, landScape->getWorldCells()->at(pIndex).posY, &guid, PB_GAME::FOOD, food.at(food.size() - 1)->tDesc);
				food.at(food.size() - 1)->guid = guid;
				if (cell != NULL) food.at(food.size() - 1)->tDesc = landScape->getWorldCells()->at(cell->pIndexInWorld).getTypeDesc(guid);

				if (cell == NULL)
				{
					//throw error

				}
				else food.at(food.size() - 1)->cell = cell;
			}
			pIndex = -1;
		}
		else
		{
			if (pIndex != -1)
			{
				bunnies.at(pIndex)->picked = true;
				landScape->cellIndexPicked = -1;
			}
		}
	}
	else if (keyDown)
	{
		if (pIndex != -1)
		{
			landScape->cellIndexPicked = -1;
			bunnies.at(pIndex)->picked  = false;
			pIndex = -1;
		}
		camera.updateCameraLocation(0.0f, 26.0f, -32.0f, 0.0f, 0.0f, 0.0f);
	}

	for (int i = 0; i < bunnies.size(); i++)
	{
		if (bunnies.at(i)->gaveBirth)
		{
			bunnies.push_back(new HBS_GAME::Actor(font, landScape->getWorldCells(), graphics, landScape, models));

			int randomX = 0;
			int randomY = 0;
			int guid    = 0;
			bool isPlaceable = false;
			PB_GAME::Cell* cell;
			do
			{
				randomX = (rand() % 32) - 16;
				randomY = (rand() % 32) - 16;

				cell = landScape->setObjectOnCell(bunnies.at(bunnies.size() - 1), randomX, randomY, &guid, PB_GAME::BUNNY, bunnies.at(bunnies.size() - 1)->tDesc);
			} while (cell == NULL);

			bunnies.at(bunnies.size() - 1)->currCell = cell;
			bunnies.at(bunnies.size() - 1)->posX = randomX;
			bunnies.at(bunnies.size() - 1)->posY = 1.0f;
			bunnies.at(bunnies.size() - 1)->posZ = randomY;
			bunnies.at(bunnies.size() - 1)->guid = guid;

			bunnies.at(bunnies.size()-1)->initalizeActor();
			bunnies.at(i)->gaveBirth = false;
		}
	}

	for (int i = 0; i < bunnies.size(); i++)
	{
		bunnies.at(i)->update(dta, camera, *landScape);
		if (bunnies.at(i)->dead) bunnies.erase(bunnies.begin()+i);
	}
}

void GameWorld::renderWorld(Camera& cam)
{
	landScape->landScapeModel->renderModel(camera);
	for (int i = 0; i < bunnies.size(); i++)
	{
		bunnies.at(i)->renderModel(cam);
	}
	for (int i = 0; i < food.size(); i++)
		food.at(i)->render(cam);

	graphics->ndcRender = true;

	Matrix ortho, viewMatrix;
	ortho.matrix = DirectX::XMMatrixOrthographicLH(1920, 1080, 0.1f, 100.0f);
	viewMatrix.matrix = DirectX::XMMatrixTranslation(0.0f, 0.0f, 1.0f);

	DirectX::XMMATRIX vp = ortho.matrix * viewMatrix.matrix;
	vp = XMMatrixTranspose(vp);
	DirectX::XMMATRIX worldMat = XMMatrixTranslation(800.0f, 450.0f, 0.0f);
	graphics->Draw(Matrix(vp), timeClock->getModelSubsets(), worldMat, timeClock->vertexBuffer, timeClock->indexBuffer);

	graphics->ndcRender = false;
}