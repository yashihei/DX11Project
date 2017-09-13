// basic.fx

Texture2D tex2d : register(t0);
Texture2D toonMapTex : register(t1);
SamplerState sampleType : register(s0);

cbuffer Params : register(b0)
{
	matrix world;
	matrix view;
	matrix proj;
	float4 diffuseColor;
	float4 ambientColor;
	float3 lightDir;
	float padding;
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
	float4 texColor, texColor2;
	float lightIntensity;
	float4 outColor = ambientColor;

	texColor = tex2d.Sample(sampleType, input.texCoord);
	lightIntensity = saturate(dot(input.normal, -lightDir));
	//FIXME:lightIntensity == 1.0f‚Ì‚Æ‚«wrap‚³‚ê‚Ä‚é
	texColor2 = toonMapTex.Sample(sampleType, float2(lightIntensity, 0));

	outColor = texColor * texColor2; 
	return outColor;
}
