Texture2D     colorMap     : register(t0);
SamplerState  colorSampler : register(s0);

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

float4 PS(PS_Input texel) : SV_TARGET
{
	float4 color = colorMap.Sample(colorSampler, texel.tex);
	clip(color - 0.03f);
	return color;
}