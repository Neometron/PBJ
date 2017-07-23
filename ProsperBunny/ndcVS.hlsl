Texture2D    colorMap     : register(t0);
SamplerState colorSampler : register(s0);

cbuffer world : register(b0)
{
	matrix worldMat;
}

cbuffer vp : register(b1)
{
	matrix vp;
}

struct VS_INPUT
{
	float4 pos     : POSITION;
	float2 tex     : TEXCOORD;
	float3 norm    : NORMAL;
	float3 diffuse : COLOR;
};

struct PS_INPUT
{
	float4 pos     : SV_POSITION;
	float2 tex     : TEXCOORD;
	float3 norm    : NORMAL;
	float3 diffuse : COLOR;
};

PS_INPUT vsMain(VS_INPUT vertices)
{
	PS_INPUT pixel = (PS_INPUT)0;
	pixel.pos = mul(vertices.pos, worldMat);
	pixel.pos = mul(pixel.pos, vp);

	pixel.tex = vertices.tex;
	pixel.norm = vertices.norm;
	pixel.diffuse = vertices.diffuse;

	return pixel;
}