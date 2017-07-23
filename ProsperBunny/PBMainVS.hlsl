cbuffer world : register(b0)
{
	matrix worldMat;
};

cbuffer vp    : register(b1)
{
	matrix viewProjMat;
};

struct Vertex
{
	float4 pos     : POSITION;
	float2 tex     : TEXCOORD;
	float3 norm    : NORMAL;
	float3 diffuse : COLOR;
};

struct Pixel
{
	float4 pos     : SV_POSITION;
	float2 tex     : TEXCOORD;
	float3 norm    : NORMAL;
	float3 diffuse : COLOR;
};

Pixel vsMain(Vertex vertice)
{
	Pixel texel = (Pixel)0;

	texel.pos     = mul(vertice.pos, worldMat);
	texel.pos     = mul(texel.pos, viewProjMat);
	texel.tex     = vertice.tex;
	texel.norm    = mul(vertice.norm, worldMat);
	texel.norm = normalize(texel.norm);

	texel.diffuse = vertice.diffuse;

	return texel;
}