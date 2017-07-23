cbuffer world : register(b0)
{
	matrix worldMat;
};

cbuffer viewProj : register(b1)
{
	matrix viewProjMat;
}

struct Vertex_Input
{
	float4 pos     : POSITION;
	float2 tex     : TEXCOORD;
	float3 norm    : NORMAL;
	float3 diffuse : COLOR;
};

struct Pixel_Input
{
	float4 pos     : SV_POSITION;
	float2 tex     : TEXCOORD;
	float3 norm    : NORMAL;
	float3 diffuse : COLOR;
};

Pixel_Input vsMain(Vertex_Input vertices)
{
	Pixel_Input pixel = (Pixel_Input)0;

	vertices.norm = normalize(vertices.norm);

	float4 original = mul(mul(vertices.pos, worldMat), viewProjMat);
	float4 normal = mul(mul(vertices.norm, worldMat), viewProjMat);

	normal = normalize(normal);

	pixel.pos = original + (mul(0.1, -normal));
	pixel.norm = normal;

	return pixel;
}