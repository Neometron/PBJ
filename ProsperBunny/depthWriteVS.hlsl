cbuffer world       : register(b0)
{
	matrix worldMat;
}

cbuffer lightMatrix : register(b1)
{
	matrix lightViewProj;
}

cbuffer isAnimated : register(b2)
{
	bool anim;
}

cbuffer time : register(b3)
{
	float time;
}

struct VS_Input
{
	float4 pos  : POSITION;
	float2 tex  : TEXCOORD;
	float3 norm : NORMAL;
};

struct PS_Input
{
	float4 pos   : SV_POSITION;
	float2 tex   : TEXCOORD;
	float3 norm  : NORMAL;
};

PS_Input VS(VS_Input vertex)
{
	PS_Input texel = (PS_Input)0;

	texel.pos = mul(vertex.pos, worldMat);

	if (anim)
	{
		float amplitude = 0.0f;
		amplitude = 0.02 * -3.0f;

		float4 wave = amplitude * float4(sin(time + texel.pos.x), cos(time + texel.pos.y), 0, 0);
		texel.pos += wave;
	}

	texel.pos = mul(texel.pos, lightViewProj);

	texel.tex = vertex.tex;
	return texel;
}
