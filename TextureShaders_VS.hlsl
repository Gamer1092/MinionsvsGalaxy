//--------------------------------------------------------------------------------------
// Basic Vertex Shader.
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
	float4 norm : NORMAL0;
	float2 texcoords : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
	float4 surfpos : POSITION0;
	float3 norm : NORMAL0;
	float2 texcoords : TEXCOORD0;
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
	output.norm = mul((float3)input.norm, (float3x3)World);
	output.texcoords = input.texcoords;

    return output;
}