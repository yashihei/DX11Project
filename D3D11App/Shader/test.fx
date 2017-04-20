// test.fx

cbuffer Params : register(b0)
{
	matrix world;
	matrix view;
	matrix proj;
};

struct VSInput
{
	float4 pos : POSITION;
	float4 color : COLOR;
};

struct PSInput
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

PSInput VS(VSInput input)
{
	PSInput output;

	input.pos.w = 1.0f;
	
	output.pos = mul(input.pos, world);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, proj);

	output.color = input.color;

	return output;
}

float4 PS(PSInput input) : SV_TARGET
{
	return input.color;
}
