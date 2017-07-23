#pragma once
#include <string>
#include <vector>

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include "Buffer.h"

namespace HBS_PB
{
	struct Vertex
	{
#ifdef WIN32
		DirectX::XMFLOAT4 pos;
		DirectX::XMFLOAT2 tex;
		DirectX::XMFLOAT3 norm;
		DirectX::XMFLOAT3 diffuse;

		Vertex(float posX, float posY, float posZ, float texX, float texY, float normX, float normY, float normZ, float dX, float dY, float dZ)
		{
			pos = DirectX::XMFLOAT4(posX, posY, posZ, 1.0f);
			tex = DirectX::XMFLOAT2(texX, texY);
			norm = DirectX::XMFLOAT3(normX, normY, normZ);
			diffuse = DirectX::XMFLOAT3(dX, dY, dZ);
		}
#endif

		Vertex() {};
	};

	struct Matrix
	{
		Matrix() {};
#ifdef WIN32
		DirectX::XMMATRIX matrix;
		Matrix(DirectX::XMMATRIX& MATRIX)
		{
			this->matrix = MATRIX;
		}
#endif
	};

	struct Texture
	{
		std::string textureName;
		ID3D11ShaderResourceView* srv = nullptr;

		~Texture()
		{
			//wif (srv) srv->Release();
		}
		Texture()
		{

		}
	};

	struct Subset
	{
		std::string               subsetName;
		std::string               textureName;
		std::vector<Vertex>       vertices;
		std::vector<unsigned int> indices;
		
		Texture tex;

		~Subset()
		{
			tex.~Texture();
			vertices.clear();
		}
		Subset(std::vector<Vertex> v, std::vector<unsigned int> i) 
		{
			this->vertices = v;
			this->indices = i;
		};
		Subset()
		{
			subsetName  = "";
			textureName = "";
		}
	};

	struct DirectionalLight
	{
		DirectX::XMFLOAT4 direction;
		DirectX::XMFLOAT4 diffuse;
		DirectX::XMFLOAT4 ambient;
	};

	struct EyePosition
	{
		float x, y, z, w;
		float angle;
		float pad[3];
	};
};