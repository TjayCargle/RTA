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
	float2 uv : TEXCOORD;
};

VOut VShader(float4 position : POSITION, float4 color : COLOR, float2 uv : TEXCOORD)
{
	VOut output;
	position.w = 1.0f;
	position = mul(position, model);
	position = mul(position, view);
	position = mul(position, projection);
	output.position = position;

	output.color = color;
	output.uv = uv;
	return output;
}

texture2D myTexture : register(t0);
SamplerState filter : register(s0);

float4 PShader(float4 position : SV_POSITION, float4 color : COLOR, float2 uv: TEXCOORD) : SV_TARGET
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
