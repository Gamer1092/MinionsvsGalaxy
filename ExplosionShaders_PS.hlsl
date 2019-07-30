//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

Texture2D myTexture : register(t0);
SamplerState mySampler : register(s0);

cbuffer ConstantBuffer: register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
	float4 timer;
	float4 up;
	float4 side;
	float4 origin;
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;
	float4 col : COLOR0;
	float4 surfpos : POSITION0;
	float2 texcoords : TEXCOORD0;
};

float4 main(PS_INPUT input) : SV_Target
{
	float4 color = myTexture.Sample(mySampler, input.texcoords);
	color.w *= input.col.w;
	return color;
}