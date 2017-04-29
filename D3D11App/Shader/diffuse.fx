// diffuse.fx
// ƒ‰ƒ“ƒo[ƒg‚µ‚¿‚á‚¤‚¼

Texture2D tex2d : register(t0);
SamplerState sampleType : register(s0);

cbuffer Params : register(b0)
{
	matrix world;
	matrix view;
	matrix proj;

	float4 diffuseColor;
	float3 lightDir;
	float padding;
};

struct VSInput
{
	float4 pos : POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PSInput
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL;
};

PSInput VS(VSInput input)
{
	PSInput output;

	input.pos.w = 1.0f;
	
	output.pos = mul(input.pos, world);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, proj);

	output.uv = input.uv;

	output.normal = mul(input.normal, (float3x3)world);
	output.normal = normalize(output.normal);

	return output;
}

float4 PS(PSInput input) : SV_TARGET
{
	float4 texColor;
	float lightIntensity;
	float4 outColor = { 0.2f, 0.2f, 0.2f, 1.0f };

	texColor = tex2d.Sample(sampleType, input.uv);
	lightIntensity = saturate(dot(input.normal, -lightDir));
	if (lightIntensity > 0)
		outColor += (diffuseColor * lightIntensity);
	outColor = saturate(outColor);
	outColor = outColor * texColor;

	return outColor;
}
