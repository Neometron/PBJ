#include "Model.h"
#include <fstream>
#include "GraphicsTypes.h"
using namespace HBS_PB;
using namespace HBS_MODEL;
using namespace DirectX;

Model::Model(HBS_GRAPHICS::Graphics *graphics, const char* modelName)
{
	assert(graphics  != nullptr);

	this->graphics  = graphics;
	this->modelName = modelName;

	vertexBuffer.buffer = nullptr;
	indexBuffer.buffer  = nullptr;
}

Model::Model(HBS_GRAPHICS::Graphics *graphics, std::vector<HBS_PB::Subset> mesh)
{
	assert(graphics != nullptr);

	this->graphics = graphics;
	this->mesh     = mesh;

	vertexBuffer.buffer = nullptr;
	indexBuffer.buffer  = nullptr;
	modelName           = nullptr;
}

Model::~Model()
{
	for (int i = 0; i < mesh.size(); i++)
	{
		mesh.at(i).~Subset();
	}
	vertexBuffer.~Buffer();
	indexBuffer.~Buffer();

	mesh.clear();
}

void Model::loadModel()
{
	if (modelName != nullptr) //user passed a non empty string for a model, let's try loading it.
	{
		try { createMesh(modelName); }
		catch (std::exception p) { //let's try loading a fallback model if it fails
			createQuad();
		}
	}
	else
	{
		if (mesh.size() > 0)
		{
			vertexBuffer = graphics->CreateBuffer(HBS_GRAPHICS::BufferType::VERTEX, HBS_GRAPHICS::BufferUse::STATIC, mesh);
			indexBuffer = graphics->CreateBuffer(HBS_GRAPHICS::BufferType::INDICE, HBS_GRAPHICS::BufferUse::STATIC, mesh);
		}
		else createQuad();
	}
}

void Model::createQuad()
{
	mesh.resize(1);

	mesh.at(0).vertices.push_back(Vertex(-1.0f, -1.0f, 0.0f, 0.0f, 1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f));
	mesh.at(0).vertices.push_back(Vertex(-1.0f, 1.0f, 0.0f, 0.0f, 0.0f,-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f));
	mesh.at(0).vertices.push_back(Vertex(1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f));
	mesh.at(0).vertices.push_back(Vertex(1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f));
	
	mesh.at(0).indices.push_back(0);
	mesh.at(0).indices.push_back(1);
	mesh.at(0).indices.push_back(2);
	mesh.at(0).indices.push_back(2);
	mesh.at(0).indices.push_back(3);
	mesh.at(0).indices.push_back(0);
		 
	mesh.at(0).subsetName = "Quad";

	vertexBuffer = graphics->CreateBuffer(HBS_GRAPHICS::BufferType::VERTEX, HBS_GRAPHICS::BufferUse::STATIC, mesh);
	indexBuffer = graphics->CreateBuffer(HBS_GRAPHICS::BufferType::INDICE, HBS_GRAPHICS::BufferUse::STATIC, mesh);
}

void Model::createMesh(const char* modelName)
{
	const aiScene*			 scene;
	aiMaterial               *mat;
	aiColor3D				 colorDiffuse, colorAmbient, colorSpecular;
	aiString                 textureName, materialName;

	int numOfSubsetsOfModel = 0;
	std::vector<std::string> word;
	std::ifstream            input_File;

	input_File = std::ifstream(modelName);

	colorDiffuse = aiColor3D(0.0f, 0.0f, 0.0f);
	colorAmbient = aiColor3D(0.0f, 0.0f, 0.0f);
	colorSpecular = aiColor3D(0.0f, 0.0f, 0.0f);

	unsigned int processFlags =
		aiProcess_CalcTangentSpace | // calculate tangents and bitangents if possible
		aiProcess_JoinIdenticalVertices | // join identical vertices/ optimize indexing
		aiProcess_Triangulate | // Ensure all verticies are triangulated (each 3 vertices are triangle)
		aiProcess_ConvertToLeftHanded | // convert everything to D3D left handed space (by default right-handed, for OpenGL)
		aiProcess_SortByPType |
		aiProcess_ImproveCacheLocality | // improve the cache locality of the output vertices
		aiProcess_RemoveRedundantMaterials | // remove redundant materials
		aiProcess_FindInvalidData | // detect invalid model data, such as invalid normal vectors
		aiProcess_GenUVCoords | // convert spherical, cylindrical, box and planar mapping to proper UVs
		aiProcess_TransformUVCoords | // preprocess UV transformations (scaling, translation ...)
		aiProcess_FindInstances | // search for instanced meshes and remove them by references to one master
		aiProcess_LimitBoneWeights | // limit bone weights to 4 per vertex
		aiProcess_OptimizeMeshes | // join small meshes, if possible;
		aiProcess_SplitByBoneCount | // split meshes with too many bones. Necessary for our (limited) hardware skinning shader
		0;

	scene = aiImportFile(modelName, processFlags);
	if (scene == nullptr) throw std::exception("Failed to create AssimpScene object", 301);

	//As of the time this was wrote ASSIMP didn't pull Object names from OBJ file, so this is one task that was done manually
	while (!input_File.eof())
	{
		std::string words;
		std::getline(input_File, words);
		if (!words.empty())
		{
			if (words.at(0) == 'o' && words.at(1) == ' ')
			{
				std::string MeshName = "";
				MeshName.assign(words, 2, words.length());
				word.push_back(MeshName);
			}
		}
	}

	for (int meshIndex = 0; meshIndex < scene->mNumMeshes; meshIndex++)
	{
	
		aiMesh* ptrMesh = scene->mMeshes[meshIndex]; //get a ptr to curr mesh
		aiMaterial *mat = scene->mMaterials[ptrMesh->mMaterialIndex];

		mesh.resize(scene->mNumMeshes);
	//	mesh.at(meshIndex).subsetName = word.at(meshIndex);

		//poll mesh material
		mat->Get(AI_MATKEY_COLOR_DIFFUSE, colorDiffuse);
		mat->Get(AI_MATKEY_COLOR_AMBIENT, colorAmbient);
		mat->Get(AI_MATKEY_COLOR_SPECULAR, colorSpecular);
		mat->Get(AI_MATKEY_NAME, materialName);

		if (ptrMesh->HasTextureCoords(0))
		{
			mat->GetTexture(aiTextureType_DIFFUSE, 0, &textureName);
			mesh.at(meshIndex).textureName = textureName.C_Str();
			mesh.at(meshIndex).tex = graphics->CreateImageTexture(textureName.C_Str());
		}

		for (int verticeIndex = 0; verticeIndex < ptrMesh->mNumVertices; verticeIndex++) //output vertices, normals, and uv's (if applicable)
		{

			if (ptrMesh->HasTextureCoords(0))
			{
				mesh.at(meshIndex).vertices.push_back(Vertex(ptrMesh->mVertices[verticeIndex].x, ptrMesh->mVertices[verticeIndex].y, ptrMesh->mVertices[verticeIndex].z,
					ptrMesh->mTextureCoords[0][verticeIndex].x, ptrMesh->mTextureCoords[0][verticeIndex].y,
					ptrMesh->mNormals[verticeIndex].x, ptrMesh->mNormals[verticeIndex].y, ptrMesh->mNormals[verticeIndex].z,
					colorDiffuse.r, colorDiffuse.g, colorDiffuse.b));
			}
			else
			{
				mesh.at(meshIndex).vertices.push_back(Vertex(ptrMesh->mVertices[verticeIndex].x, ptrMesh->mVertices[verticeIndex].y, ptrMesh->mVertices[verticeIndex].z,
					0.0f, 0.0f,
					ptrMesh->mNormals[verticeIndex].x, ptrMesh->mNormals[verticeIndex].y, ptrMesh->mNormals[verticeIndex].z,
					colorDiffuse.r, colorDiffuse.g, colorDiffuse.b));
			}
		}

		for (unsigned int face = 0; face < ptrMesh->mNumFaces; face++) //output indices
			for (unsigned int indx = 0; indx < 3; indx++)
				mesh.at(meshIndex).indices.push_back(ptrMesh->mFaces[face].mIndices[indx]);

		//mesh.at(meshIndex).subsetName = word.at(meshIndex);

	}	//As of the time this was wrote ASSIMP didn't pull Object names from OBJ file, so this is one task that was done manually

	vertexBuffer = graphics->CreateBuffer(HBS_GRAPHICS::BufferType::VERTEX, HBS_GRAPHICS::BufferUse::STATIC, mesh);
	indexBuffer  = graphics->CreateBuffer(HBS_GRAPHICS::BufferType::INDICE, HBS_GRAPHICS::BufferUse::STATIC, mesh);
}

triangleStructure Model::checkIfPicked(HBS::HBS_MATH::Vector& worldRay, HBS::HBS_MATH::Vector& worldRayDir, Camera& cam, const HBS_WINDOW::Handle& handle)
{
	triangleStructure ts = cam.pick(worldRay, worldRayDir, mesh, Matrix(worldMat));
	if (ts.distanceToHit < FLT_MAX) return ts;
	else                            return triangleStructure(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
}

const std::vector<Subset>& Model::getModelSubsets() const
{
	return mesh;
}

void Model::renderModel(Camera& cam)
{
	Matrix vp = cam.getViewProj();
	graphics->Draw(vp, mesh, worldMat, vertexBuffer, indexBuffer);
}