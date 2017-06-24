// A constant buffer that stores the three basic column-major matrices for composing geometry.
cbuffer TJMVPCBuffer : register(b0)
{
	matrix model;
	matrix view;
	matrix projection;
};

struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float3 norm : NORMAL;
	float2 uv : TEXCOORD;
};

VOut VShader(float4 position : POSITION, float4 color : COLOR, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	VOut output;
	position.w = 1.0f;
	position = mul(position, model);
	position = mul(position, view);
	position = mul(position, projection);
	output.position = position;
	output.norm = normal;
	output.color = color;
	output.uv = uv;
	return output;
}

texture2D myTexture : register(t0);
SamplerState filter : register(s0);
struct Light
{
	float1 lightRatio;
	float4 pos;
	float4 dir;
	float4 color;
};
float4 PShader(float4 position : SV_POSITION, float4 color : COLOR, float3 normal : NORMAL, float2 uv: TEXCOORD) : SV_TARGET
{
	Light spotLight;

spotLight.pos.x = 30.2f;
spotLight.pos.y = 2.5f;
spotLight.pos.z = 20.0f;
spotLight.pos.w = 1;

spotLight.dir.x = 1.0f;
spotLight.dir.y = 1.0f;
spotLight.dir.z = 1.0f;
spotLight.dir.w = 1.0f;
float surfaceRatio = 0.0f;
float4 coneDir = float4(0, 0, 0, 0);
coneDir.x = 0.333f;
coneDir.y = 0.333f;
coneDir.z = -0.333f;
coneDir.w = 1.0f;
float spotFactor = 1.0f;
spotLight.dir = normalize(spotLight.pos - position);

float coneRatio = 0.0f;
coneRatio = dot(-coneDir, position);
coneRatio = clamp(coneRatio, 1, 0);
surfaceRatio = dot(-spotLight.dir, coneDir);
surfaceRatio = clamp(surfaceRatio, 1, 0.2);
if (surfaceRatio > coneRatio)
{
	spotFactor = 1.0f;
}
else
{
	spotFactor = 0.0f;
}
spotLight.lightRatio = 1.0f;

spotLight.lightRatio = dot(spotLight.dir, normal);
spotLight.lightRatio = clamp(spotLight.lightRatio, 1, 0.5);

spotLight.color.r = 1.0f;
spotLight.color.g = 1.0f;
spotLight.color.b = 1.0f;
spotLight.color.a = 1.0f;
float range2 = 1.8f;
float Dist2 = length(spotLight.dir);
float rangeAtt2 = 1 - clamp(Dist2 / range2, 1, 0.2);
spotLight.color.r = 0.0f;
spotLight.color.b = 1.0f *spotLight.lightRatio * rangeAtt2 * spotFactor;
spotLight.color.g = 0.0f;
spotLight.color.a = 1.0f *spotLight.lightRatio* rangeAtt2;


	float4 returnedColor = myTexture.Sample(filter,uv);
	if (returnedColor.a <= 0)
	{
	return color;
	}
	else
	{
		return returnedColor;// *spotLight.color;
	}

}
