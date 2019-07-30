//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

cbuffer ConstantBuffer: register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR0;
};

float4 main(VS_OUTPUT input) : SV_Target
{
	return input.color;
}