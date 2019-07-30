//--------------------------------------------------------------------------------------
// Basic Vertex Shader.
//--------------------------------------------------------------------------------------
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

struct VS_INPUT
{
	float4 pos : POSITION;
	float4 col : COLOR0;
	float4 norm : NORMAL0;
	float2 texcoords : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
	float4 col : COLOR0;
	float2 texcoords : TEXCOORD0;
	float  id : TEXCOORD1;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;

	float4 norm = mul(input.norm, World);
	float4 pos = origin;
	pos = mul(pos, World);
	
	float circtime = timer.x;// - 5 * floor(timer.x / 5);
	if (circtime > 5.0f)
			circtime -= 5.0f;

	float time = circtime;
	float slow = 0.9;
	
	pos.x += norm.x*time*slow; 
	pos.y += norm.y*time*slow;
	pos.z += norm.z*time*slow;
	
    output.pos = pos;

	float alpha = 1.0f;

	output.col = float4(1, 1, 1, alpha);
	output.id = input.texcoords.x; // pass in id through texcoords
	output.texcoords = input.texcoords;
    return output;
}