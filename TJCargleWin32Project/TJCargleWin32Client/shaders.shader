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

};

VOut VShader(float4 position : POSITION, float4 color : COLOR )
{
	VOut output;
	position.w = 1.0f;
	position = mul(position, model);
	position = mul(position, view);
	position = mul(position, projection);
	output.position = position;

	output.color = color;

	return output;
}

//SamplerState MeshTextureSampler
//{
//	Filter = MIN_MAG_MIP_LINEAR;
//	AddressU = Wrap;
//	AddressV = Wrap;
//};

//texture2D myTexture : register(t0);
//SamplerState filter : register(s0);

float4 PShader(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
{
	//float4 returnedColor = myTexture.Sample(filter,uv);
	//if (returnedColor.a <= 0)
//	{
	return color;
//	}

}
