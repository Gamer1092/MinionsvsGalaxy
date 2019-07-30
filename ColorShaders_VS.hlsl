//--------------------------------------------------------------------------------------
// Basic Vertex and Pixel Shaders.
//
//--------------------------------------------------------------------------------------

cbuffer ConstantBuffer: register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
};

struct VS_INPUT
{
	float4 pos : POSITION;
	float4 color : COLOR0;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
	float4 pos = float4(input.pos.x, input.pos.y, input.pos.z, 1.0);
	output.pos = mul(pos, World);
	output.pos = mul(output.pos, View);
	output.pos = mul(output.pos, Projection);
    output.color = input.color;

    return output;
}