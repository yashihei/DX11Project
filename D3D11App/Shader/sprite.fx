// test.fx

Texture2D tex2d : register(t0);
SamplerState sampleType : register(s0);

cbuffer Params : register(b0)
{
	matrix world;
	matrix view;
	matrix proj;
};

struct VSInput
{
	float4 pos : POSITION;
	float2 uv : TEXCOORD0;
};

struct PSInput
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};

PSInput VS(VSInput input)
{
	PSInput output;

	input.pos.w = 1.0f;
	
	output.pos = mul(input.pos, world);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, proj);

	output.uv = input.uv;

	return output;
}

float4 PS(PSInput input) : SV_TARGET
{
	return tex2d.Sample(sampleType, input.uv);
}
