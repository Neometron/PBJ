Texture2D    colorMap     : register(t0);
SamplerState colorSampler : register(s0);

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

float4 psMain(PS_INPUT texel) : SV_TARGET
{
	if (colorMap.Sample(colorSampler, texel.tex).a < 0.1f) discard;
	return colorMap.Sample(colorSampler, texel.tex);
}