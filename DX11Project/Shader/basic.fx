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

	output.normal = mul(input.normal, (float3x3)world);
	output.normal = normalize(output.normal);

	output.texCoord = input.texCoord;

	return output;
}

float3 LambertLighting(
	float3 lightNormal,
	float3 surfaceNormal,
	float3 lightColor,
	float3 pixelColor
)
{
	// compute amount of contribution per light
	float diffuseAmount = saturate(dot(lightNormal, surfaceNormal));
	float3 diffuse = diffuseAmount * lightColor * pixelColor;
	return diffuse;
}

float4 CombineRGBWithAlpha(float3 rgb, float a) 
{ 
	return float4(rgb.r, rgb.g, rgb.b, a); 
}

float4 PS(PSInput input) : SV_TARGET
{
	float4 outColor;

	float3 temp = ambientMaterial.rgb * ambientLight.rgb;
	temp += LambertLighting(-lightDirection, input.normal, diffuseLight.rgb, diffuseMaterial.rgb);
	temp = saturate(temp);

	float3 temp2 = tex2d.Sample(sampleType, input.texCoord).rgb * temp;
	float temp3 = tex2d.Sample(sampleType, input.texCoord).a * diffuseMaterial.a;
	outColor = CombineRGBWithAlpha(temp2, temp3);

	return outColor;
}
