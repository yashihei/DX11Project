// basic.fx

Texture2D tex2d : register(t0);
Texture2D toonMapTex : register(t1);
SamplerState sampleType : register(s0);

cbuffer ObjectParams : register(b0)
{
	matrix world;
	matrix view;
	matrix proj;
};

cbuffer LightParams : register(b1)
{
	float4 diffuseLight;
	float4 ambientLight;
	float3 lightDirection;
	float padding;
};

cbuffer MaterialParams : register(b2)
{
	float4 diffuseMaterial;
	float4 ambientMaterial;
	float4 specularMaterial;
	float4 power;
};

struct VSInput
{
	float4 pos : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD0;
};

struct PSInput
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD0;
};

PSInput VS(VSInput input)
{
	PSInput output;

	input.pos.w = 1.0f;
	
	output.pos = mul(input.pos, world);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, proj);

	output.texCoord = input.texCoord;

	output.normal = mul(input.normal, (float3x3)world);
	output.normal = normalize(output.normal);

	return output;
}

float4 PSHalf(PSInput input) : SV_TARGET
{
	float4 outColor;

	// half lambert
	float diffuseAmount = dot(-lightDirection, input.normal);
	diffuseAmount = diffuseAmount * 0.5f + 0.5f;
	diffuseAmount *= diffuseAmount;

	float4 toonColor = toonMapTex.Sample(sampleType, float2(diffuseAmount, diffuseAmount));

	float4 diffuseColor = diffuseMaterial;
	diffuseColor *= tex2d.Sample(sampleType, input.texCoord);
	diffuseColor *= diffuseLight;

	outColor = diffuseColor * toonColor;

	return outColor;
}
