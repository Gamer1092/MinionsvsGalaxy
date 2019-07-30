//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

Texture2D myTexture : register(t0);
SamplerState mysampler : register(s0);

cbuffer ConstantBuffer: register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float2 texcoords : TEXCOORD0;
};

float4 main(VS_OUTPUT input) : SV_Target
{
	float4 texCol = myTexture.Sample(mysampler, input.texcoords);
	return texCol;
}