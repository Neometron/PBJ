struct Pixel_Input
{
	float4 pos  : SV_POSITION;
	float2 tex     : TEXCOORD;
	float3 norm : NORMAL;
	float3 diffuse : COLOR;
};

float4 psMain(Pixel_Input pix) : SV_TARGET
{
	return float4(0.0f, 0.0f, 0.0f, 0.0f);
}