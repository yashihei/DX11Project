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

float4 PS(PSInput input) : SV_TARGET
{
	//TODO:light対応
	//TODO:material対応
	float4 texColor, texColor2;
	float lightIntensity;
	float4 outColor;

	texColor = tex2d.Sample(sampleType, input.texCoord);
	lightIntensity = saturate(dot(input.normal, -lightDirection));

	//FIXME:lightIntensity == 1.0fのときwrapされてる
	texColor2 = toonMapTex.Sample(sampleType, float2(lightIntensity, 0));

	outColor = texColor * texColor2; 
	return outColor;
}
