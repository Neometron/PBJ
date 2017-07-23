#include "Font.h"
using namespace HBS;
using namespace HBS_PB;

Font::Font(HBS_GRAPHICS::Graphics* graphics)
{
	this->graphics = graphics;

	orthographicMatrix.matrix = DirectX::XMMatrixOrthographicLH(1920, 1080, 0.1f, 100.0f);

	std::vector<Vertex> vertices;
	vertices.push_back(Vertex(-40.0f, -40.0f, 0.0f, 0.0f, 1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f));
	vertices.push_back(Vertex(-40.0f,  40.0f, 0.0f, 0.0f, 0.0f, -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f));
	vertices.push_back(Vertex( 40.0f,  40.0f, 0.0f, 1.0f, 0.0f,  1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f));
	vertices.push_back(Vertex( 40.0f, -40.0f, 0.0f, 1.0f, 1.0f,  1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f));

	std::vector<unsigned int> indices;
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(0);

	std::vector<HBS_PB::Subset> mesh;
	mesh.push_back(HBS_PB::Subset(vertices, indices));

	fQuad = new HBS_MODEL::Model(graphics, mesh);
	fontTextures.resize(26);

	fontTextures.at(0).textureName = "dds//a.dds";
	fontTextures.at(1).textureName = "dds//b.dds";
	fontTextures.at(2).textureName = "dds//c.dds";
	fontTextures.at(3).textureName = "dds//d.dds";
	fontTextures.at(4).textureName = "dds//e.dds";
	fontTextures.at(5).textureName = "dds//f.dds";
	fontTextures.at(6).textureName = "dds//g.dds";
	fontTextures.at(7).textureName = "dds//h.dds";
	fontTextures.at(8).textureName = "dds//i.dds";
	fontTextures.at(9).textureName = "dds//j.dds";
	fontTextures.at(10).textureName = "dds//k.dds";
	fontTextures.at(11).textureName = "dds//l.dds";
	fontTextures.at(12).textureName = "dds//m.dds";
	fontTextures.at(13).textureName = "dds//n.dds";
	fontTextures.at(14).textureName = "dds//o.dds";
	fontTextures.at(15).textureName = "dds//p.dds";
	fontTextures.at(16).textureName = "dds//q.dds";
	fontTextures.at(17).textureName = "dds//r.dds";
	fontTextures.at(18).textureName = "dds//s.dds";
	fontTextures.at(19).textureName = "dds//t.dds";
	fontTextures.at(20).textureName = "dds//u.dds";
	fontTextures.at(21).textureName = "dds//v.dds";
	fontTextures.at(22).textureName = "dds//w.dds";
	fontTextures.at(23).textureName = "dds//x.dds";
	fontTextures.at(24).textureName = "dds//y.dds";
	fontTextures.at(25).textureName = "dds//z.dds";

	for (int i = 0; i < 26; i++) fontTextures.at(i) = graphics->CreateImageTexture(fontTextures.at(i).textureName.c_str());
	posXOffset = 0.0f;
}

Font::~Font()
{
	graphics = nullptr;
	fQuad->~Model();

	for (int i = 0; i < fontTextures.size(); i++)
	{
		fontTextures.at(i).~Texture();
	}
}

void Font::loadFont()
{
	fQuad->loadModel();
}

void Font::drawText(std::string text, int x, int y)
{
	posXOffset = 0.0f;
	for (int i = 0; i < text.size(); i++)
	{
		switch (text.at(i))
		{
		case 'a':
			fQuad->mesh.at(0).tex = fontTextures.at(0);
			break;
		case 'b':
			fQuad->mesh.at(0).tex = fontTextures.at(1);
			break;
		case 'c':
			fQuad->mesh.at(0).tex = fontTextures.at(2);
			break;
		case 'd':
			fQuad->mesh.at(0).tex = fontTextures.at(3);
			break;
		case 'e':
			fQuad->mesh.at(0).tex = fontTextures.at(4);
			break;
		case 'f':
			fQuad->mesh.at(0).tex = fontTextures.at(5);
			break;
		case 'g':
			fQuad->mesh.at(0).tex = fontTextures.at(6);
			break;
		case 'h':
			fQuad->mesh.at(0).tex = fontTextures.at(7);
			break;
		case 'i':
			fQuad->mesh.at(0).tex = fontTextures.at(8);
			break;
		case 'j':
			fQuad->mesh.at(0).tex = fontTextures.at(9);
			break;
		case 'k':
			fQuad->mesh.at(0).tex = fontTextures.at(10);
			break;
		case 'l':
			fQuad->mesh.at(0).tex = fontTextures.at(11);
			break;
		case 'm':
			fQuad->mesh.at(0).tex = fontTextures.at(12);
			break;
		case 'n':
			fQuad->mesh.at(0).tex = fontTextures.at(13);
			break;
		case 'o':
			fQuad->mesh.at(0).tex = fontTextures.at(14);
			break;
		case 'p':
			fQuad->mesh.at(0).tex = fontTextures.at(15);
			break;
		case 'q':
			fQuad->mesh.at(0).tex = fontTextures.at(16);
			break;
		case 'r':
			fQuad->mesh.at(0).tex = fontTextures.at(17);
			break;
		case 's':
			fQuad->mesh.at(0).tex = fontTextures.at(18);
			break;
		case 't':
			fQuad->mesh.at(0).tex = fontTextures.at(19);
			break;
		case 'u':
			fQuad->mesh.at(0).tex = fontTextures.at(20);
			break;
		case 'v':
			fQuad->mesh.at(0).tex = fontTextures.at(21);
			break;
		case 'w':
			fQuad->mesh.at(0).tex = fontTextures.at(22);
			break;
		case 'x':
			fQuad->mesh.at(0).tex = fontTextures.at(23);
			break;
		case 'y':
			fQuad->mesh.at(0).tex = fontTextures.at(24);
			break;
		case 'z':
			fQuad->mesh.at(0).tex = fontTextures.at(25);
			break;

		}

		Matrix worldMat = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f) * XMMatrixTranslation(x+posXOffset, y-200.0f, 0.0f);
		viewMatrix = XMMatrixTranslation(0.0f, 0.0f, 1.0f);

		DirectX::XMMATRIX vp = orthographicMatrix.matrix * viewMatrix.matrix;
		vp = DirectX::XMMatrixTranspose(vp);

		graphics->ndcRender = true;
		graphics->Draw(Matrix(vp), fQuad->getModelSubsets(), worldMat.matrix, fQuad->vertexBuffer, fQuad->indexBuffer);
		graphics->ndcRender = false;

		posXOffset += 40.0f;
	}
}