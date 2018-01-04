// blur.fx

Texture2D g_diffuseTex : register(t0);
SamplerState g_sampleType : register(s0);

cbuffer ObjectParams : register(b0)
{
	matrix g_world;
	matrix g_view;
	matrix g_proj;
};

struct VSInput
{
	float4 pos : POSITION;
	float2 texCoord : TEXCOORD0;
	float4 color : COLOR0;
};

struct PSInput
{
	float4 pos : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float4 color : COLOR0;
};

PSInput VS(VSInput input)
{
	PSInput output;

	input.pos.w = 1.0f;
	
	output.pos = mul(input.pos,  g_world);
	output.pos = mul(output.pos, g_view);
	output.pos = mul(output.pos, g_proj);

	output.texCoord = input.texCoord;
	output.color = input.color;

	return output;
}

float4 PS(PSInput input) : SV_TARGET
{
	return g_diffuseTex.Sample(g_sampleType, input.texCoord) * input.color;
}
