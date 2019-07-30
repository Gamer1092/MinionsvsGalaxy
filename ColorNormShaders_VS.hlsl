//--------------------------------------------------------------------------------------
// Basic Vertex and Pixel Shaders.
//
//--------------------------------------------------------------------------------------

cbuffer ConstantBuffer: register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
	float4 lightpos;
	float4 eyepos;
};

struct VS_INPUT
{
	float4 pos : POSITION;
	float4 color : COLOR0;
	float4 norm : NORMAL0;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR0;
	float3 normal : NORMAL0;
	float4 surfpos : POSITION0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
	output.surfpos = mul(input.pos, World);
	output.pos = mul(input.pos, World);
	output.pos = mul(output.pos, View);
	output.pos = mul(output.pos, Projection);
    output.color = input.color;
	output.normal = mul((float3)input.norm, (float3x3)World);

    return output;
}