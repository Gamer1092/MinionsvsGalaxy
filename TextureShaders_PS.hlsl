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
	float4 lightpos;
	float4 eyepos;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
	float4 surfpos : POSITION0;
	float3 normal : NORMAL0;
	float2 texcoords : TEXCOORD0;
};

float4 main(VS_OUTPUT input) : SV_Target
{
    float3 eyee;
	float3 lighte;

	eyee = float3(eyepos.x, eyepos.y, eyepos.z);
	lighte = float3(lightpos.x, lightpos.y, lightpos.z);

	float3 L, N, V, R;
	
	N = normalize(input.normal);
	V = eyee - (float3)input.surfpos;
	V = normalize(V);
	L = lighte - (float3)input.surfpos;
	L = normalize(L);

	//Light source 
	float diffuse = saturate(dot(N,L));

	R = 2*dot(L,N)*N - L;

	float spec = saturate(dot(V,R));
	spec = pow(spec, 99);

	float amb = 0.1f;
	
	float I = 0.7*diffuse + 1*spec + 1*amb;

	//final intensity calculation
	float4 texCol = myTexture.Sample(mysampler, input.texcoords);
	float3 cr = float3(texCol.x, texCol.y, texCol.z);

	return float4(I*cr, 1.0);
}