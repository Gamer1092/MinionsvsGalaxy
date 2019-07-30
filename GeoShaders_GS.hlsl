// same constant buffer as used for other shaders
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

// vertex data obtained from the vertex shader.
struct GS_INPUT
{
	float4 wPos : SV_POSITION;
	float4 col : COLOR0;
	float2 texcoords : TEXCOORD0;
	float  id : TEXCOORD1;
};

// Per-pixel color data passed through the pixel shader.
struct GS_OUTPUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR0;
	float4 surfpos : POSITION0;
	float2 texcoord : TEXCOORD0;
};

// Geo shader, makes quad
[maxvertexcount(4)]
void main(point GS_INPUT input[1], inout TriangleStream<GS_OUTPUT> triStream)
{
	float4 v[4];

	float sz = 0.1f;

	v[0] = float4(input[0].wPos + sz*(-side+up));
	v[1] = float4(input[0].wPos + sz*(side+up));
	v[2] = float4(input[0].wPos + sz*(-side-up));
	v[3] = float4(input[0].wPos + sz*(side-up));

	GS_OUTPUT output;
	int fid = (int)floor(input[0].id * 4);
	for (uint i = 0; i < 4; i++)
	{
		output.surfpos = v[i];
		output.pos = mul(v[i], View);
		output.pos = mul(output.pos, Projection);
		output.texcoord = float2((floor(i / 2)*0.5 + 0.5*(fid/2) + timer.y)/4.0, (float)(i % 2)*0.5 + 0.5*(fid%2));
		output.color = float4(0, 0, 0, input[0].col.w);
		triStream.Append(output);
	}
}
