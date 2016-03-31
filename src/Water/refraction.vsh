
cbuffer D3DMVPMatrix : register(b0)
{
  float4x4 model;
  float4x4 view;
  float4x4 proj;
};

cbuffer ClipPlaneBufer
{
  float4 clipPlane = float4(0.0f, -1.0f, 0.0f, 2.75f + 0.1f);
};

struct VS_IN
{
   float4 Pos    : POSITION;
   float2 Tex    : TEXCOORD;
   float3 Normal : NORMAL;
};

struct VS_OUT
{
  float4 Pos        : SV_POSITION;
  float2 Tex        : TEXCOORD;
  float3 Normal     : NORMAL;
  float clipDis : SV_ClipDistance0;
};

VS_OUT VS( VS_IN vs_in)
{
    VS_OUT vs_out;

	vs_in.Pos.w = 1.0f;
	vs_out.Pos = mul(vs_in.Pos, model);
	vs_out.Pos = mul(vs_out.Pos, view);
	vs_out.Pos = mul(vs_out.Pos, proj);

	vs_out.Tex = vs_in.Tex;
	vs_out.Normal = mul(vs_in.Normal, (float3x3)model);
	vs_out.Normal = normalize(vs_in.Normal);
    vs_out.clipDis = dot( mul(vs_in.Pos, model), clipPlane);

   return vs_out;	
}