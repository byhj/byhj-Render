 
cbuffer MatrixBuffer
{
	float4x4 model;
	float4x4 view;
	float4x4 proj;
};

struct VS_IN
{
    float4 Pos       : POSITION;
	float3 Normal    : NORMAL;
    float2 Tex       : TEXCOORD0;
	float3 Tangent   : TANGENT;
	float3 BiTangent : BITANGENT;
};

struct VS_OUT
{
    float4 Pos       : SV_POSITION;
	float3 Normal    : NORMAL;
    float2 Tex       : TEXCOORD0;
	float3 Tangent   : TANGENT;
	float3 BiTangent : BITANGENT;
	float3 WorldPos  : POSITION;
};

VS_OUT Model_VS( VS_IN vs_in )
{	
 
   VS_OUT vs_out;

   vs_out.Pos = mul(vs_in.Pos, model);
   vs_out.Pos = mul(vs_out.Pos, view);
   vs_out.Pos = mul(vs_out.Pos, proj);

   vs_out.Normal    = mul(vs_in.Normal,  (float3x3)model );
   vs_out.Tangent   = mul(vs_in.Tangent, (float3x3)model );
   vs_out.BiTangent = mul(vs_in.BiTangent, (float3x3)model );
   vs_out.WorldPos  = mul(vs_in.Pos, model);

   vs_out.Tex = vs_in.Tex;
 
   return vs_out;
}
