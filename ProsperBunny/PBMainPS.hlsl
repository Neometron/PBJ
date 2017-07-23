Texture2D    colorMap     : register(t0);
SamplerState colorSampler : register(s0);

cbuffer dirLight : register(b0)
{
	float4 lightDir;
	float4 diffuseLight;
	float4 ambientLight;
};

cbuffer ePos     : register(b1)
{
	float4 ePosWorldSpace;
	float  angleAmount;
}

struct Pixel
{
	float4 pos     : SV_POSITION;
	float2 tex     : TEXCOORD;
	float3 norm    : NORMAL;
	float3 diffuse : COLOR;
};

float4 psMain(Pixel texel) : SV_TARGET
{
	if (colorMap.Sample(colorSampler, texel.tex).a < 0.1f) discard;

	float3 colorVal;
	float lightAmnt = saturate(dot(texel.norm, lightDir));
	colorVal = colorMap.Sample(colorSampler, texel.tex) + (diffuseLight*lightAmnt);
	//colorVal = (diffuseLight*lightAmnt);


	if (lightAmnt < 0.1f)
		colorVal = float4(0.45f, 0.45f, 0.45f, 0.1f) * colorVal;
	else if (lightAmnt >= 0.1f && lightAmnt < 0.1050f)
		colorVal = float4(0.5f, 0.45f, 0.425f, 1.0f) * colorVal;
	else if (lightAmnt >= 0.105f && lightAmnt < 0.11f)
		colorVal = float4(0.6f, 0.5f, 0.45f, 1.0f) * colorVal;
	else if (lightAmnt >= 0.11f && lightAmnt < 0.15f)
		colorVal = float4(0.75f, 0.55f, 0.475f, 1.0f) * colorVal;
	else
		colorVal = float4(0.8f, 0.6f, 0.5f, 1.0f) * colorVal;

	return float4(colorVal, 1.0f);
}