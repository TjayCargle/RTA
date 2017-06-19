
struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

VOut VShader(float4 position : POSITION, float4 color : COLOR)
{
	VOut output;


	output.position = position;
	output.color = color;

	return output;
}

SamplerState MeshTextureSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

texture2D myTexture : register(t0);
SamplerState filter : register(s0);

float4 PShader(float4 position : SV_POSITION, float4 color : COLOR, float2 uv : TEXTCOORD) : SV_TARGET
{
	float4 returnedColor = myTexture.Sample(filter,uv);
	if (returnedColor.a <= 0)
	{
	return color;
	}
	else
	{
		return returnedColor;
	}
}
