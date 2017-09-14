// basic.fx

Texture2D tex2d : register(t0);
SamplerState sampleType : register(s0);

cbuffer ObjectParams : register(b0)
{
	matrix world;
	matrix view;
	matrix proj;
};

cbuffer LightParams : register(b1)
{
	float4 lightDiffuseColor;
	float4 lightAmbientColor;
	float3 lightDirection;
	float padding;
};

cbuffer MaterialParams : register(b2)
{
	float4 diffuseColor;
	float4 ambientColor;
	float4 specularColor;
	float4 power;
};

struct VSInput
{
	float4 pos : POSITION;
	float2 texCoord : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PSInput
{
	float4 pos : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float3 normal : NORMAL;
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

float4 PS(PSInput input) : SV_TARGET
{
	float4 texColor;
	float lightIntensity;
	float4 outColor = lightAmbientColor;

	texColor = tex2d.Sample(sampleType, input.texCoord);
	lightIntensity = saturate(dot(input.normal, -lightDirection));
	if (lightIntensity > 0)
		outColor += (lightDiffuseColor * lightIntensity);
	outColor = saturate(outColor);
	outColor = outColor * texColor;

	return outColor;
}
