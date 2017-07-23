#pragma once
#include "Buffer.h"
#include "Graphics.h"
#include "Camera.h"
#include "Window.h"
#include "MathTypes.h"

#ifdef WIN32
#include <DirectXMath.h>
#endif

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#pragma comment (lib, "assimp.lib")

namespace HBS_PB
{
	namespace HBS_MODEL
	{
		enum Mesh
		{
			QUAD,
			CQUAD,
			CUBE,
			MESH
		};
		class Model
		{
		public:
			Model(HBS_GRAPHICS::Graphics*, const char*);
			Model(HBS_GRAPHICS::Graphics*, std::vector<HBS_PB::Subset> mesh);
			Model() {};
			~Model();
		public:
			void                               loadModel();
			void                               renderModel(Camera& cam);
			const std::vector<HBS_PB::Subset>& getModelSubsets() const;
			triangleStructure                  checkIfPicked(HBS::HBS_MATH::Vector& worldRay, HBS::HBS_MATH::Vector& worldRayDir, Camera& cam, const HBS_WINDOW::Handle& handle);
		private:
			void createQuad();
			void createCQuad(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
			void createMesh(const char*);
		public:
#ifdef WIN32
			DirectX::XMMATRIX         worldMat;
			DirectX::XMMATRIX  getWorldMatrix() { return worldMat; }
#endif
			std::vector<HBS_PB::Subset> mesh;
			HBS_GRAPHICS::Buffer    vertexBuffer;
			HBS_GRAPHICS::Buffer    indexBuffer;
		private:
			HBS_GRAPHICS::Graphics* graphics;
			Mesh                    meshType;
			const char*             modelName;
		};
	};
};
