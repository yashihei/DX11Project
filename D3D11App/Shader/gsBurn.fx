// gstest.fx

Texture2D tex2d : register(t0);
SamplerState sampleType : register(s0);

cbuffer Params : register(b0)
{
	matrix world;
	matrix view;
	matrix proj;
	float4 diffuseColor;
	float3 lightDir;
	float time;
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
	
	output.pos = input.pos;
	output.uv = input.uv;
	//output.normal = input.normal;
	output.normal = normalize(mul(input.normal, (float3x3)world));

	return output;
}

void SetRotate(float3 rotang, out float3x3 mat)
{
	float2 roty;
	float2 rotz;
	sincos(rotang.y, roty.x, roty.y);
	sincos(rotang.z, rotz.x, rotz.y);
	mat._11 = rotz.y * roty.y;
	mat._12 = rotz.x * roty.y;
	mat._13 = -roty.x;
	mat._21 = -rotz.x;
	mat._22 = rotz.y;
	mat._23 = 0.0f;
	mat._31 = roty.x;
	mat._32 = 0.0f;
	mat._33 = roty.y;
}

[maxvertexcount(3)]
void GS(triangle PSInput input[3], inout TriangleStream<PSInput> TriStream)
{
	PSInput output;

    float3 faceEdgeA = input[1].pos - input[0].pos;
    float3 faceEdgeB = input[2].pos - input[0].pos;
    float3 faceNormal = normalize( cross(faceEdgeA, faceEdgeB) );
	float t = abs(sin(time*0.5f)) * 25 - 5.0f;
	float3 ExplodeAmt = faceNormal*t;
    float3 center = (input[0].pos.xyz + input[1].pos.xyz + input[2].pos.xyz)/3.0;

	if (t < 0) ExplodeAmt = 0;

	//float3 ang = float3(0, 0, t);
	//if (t < 0) ang = float3(0, 0, 0);
	//float3x3 rotmat;
	//SetRotate(ang, rotmat);
	
	for (int i = 0; i < 3; i++) {
		output.pos = input[i].pos + float4(ExplodeAmt, 0);
		output.pos = mul(output.pos, world);
		output.pos = mul(output.pos, view);
		output.pos = mul(output.pos, proj);
        output.normal = input[i].normal;
        output.uv = input[i].uv;
        TriStream.Append( output );
	}

    TriStream.RestartStrip();
}

float4 PS(PSInput input) : SV_TARGET
{
	float4 texColor;
	float lightIntensity;
	float4 outColor;

	texColor = tex2d.Sample(sampleType, input.uv);
	lightIntensity = saturate(dot(input.normal, -lightDir));
	outColor = saturate(diffuseColor * lightIntensity);
	outColor = outColor * texColor;

	return outColor;
}
