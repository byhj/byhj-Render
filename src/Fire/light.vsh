///////////////////////////////////////////////////////////
struct Matrix
{
	float4x4 model;
	float4x4 view;
	float4x4 proj;
};

cbuffer d3d::MatrixBuffer : register(b0)
{
    Matrix cbMatrix;
};

//////////////////////////////////////////////////////////
struct Noise
{
    float  frameTime;
    float3 scrollSpeeds;
    float3 scales;
    float  padding;
};

cbuffer NoiseBuffer   : register(b1)
{
   Noise cbNoise;
};
/////////////////////////////////////////////////////////////
struct VS_IN
{
	 float4 Pos     : POSITION;
	 float2 Tex     : TEXCOORD0;
	 float3 Normal  : NORMAL;
};

struct VS_OUT
{
    float4 Pos  : SV_POSITION;
	float2 Tex  : TEXCOORD0;
   	float2 Tex1 : TEXCOORD1;
	float2 Tex2 : TEXCOORD2;
	float2 Tex3 : TEXCOORD3;
};

////////////////////////////////////////////////////////
VS_OUT VS(VS_IN vs_in)
{	
 
   VS_OUT vs_out;
   vs_out.Pos.w = 1.0f;
   vs_out.Pos = mul(vs_in.Pos,  cbMatrix.model);
   vs_out.Pos = mul(vs_out.Pos, cbMatrix.view);
   vs_out.Pos = mul(vs_out.Pos, cbMatrix.proj);


	// Compute texture coordinates for first noise texture using the first scale and upward scrolling speed values.
   vs_out.Tex1 = (vs_in.Tex * cbNoise.scales.x);
   vs_out.Tex1.y = vs_out.Tex1.y + (cbNoise.frameTime * cbNoise.scrollSpeeds.x);

   // Compute texture coordinates for second noise texture using the second scale and upward scrolling speed values.
   vs_out.Tex2 = (vs_in.Tex * cbNoise.scales.y);
   vs_out.Tex2.y = vs_out.Tex2.y + (cbNoise.frameTime * cbNoise.scrollSpeeds.y);

   // Compute texture coordinates for third noise texture using the third scale and upward scrolling speed values.
   vs_out.Tex3 = (vs_in.Tex * cbNoise.scales.z);
   vs_out.Tex3.y = vs_out.Tex3.y + (cbNoise.frameTime * cbNoise.scrollSpeeds.z);
   	
   vs_out.Tex = vs_in.Tex;

   return vs_out;
}
