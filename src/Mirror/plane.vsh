
cbuffer d3d::MatrixBuffer : register(b0)
{
  float4x4 model;
  float4x4 view;
  float4x4 proj;
  float4x4 reflection;
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
  float4 ReflectPos : TEXCOORD1;
};

VS_OUT VS( VS_IN vs_in)
{
    VS_OUT vs_out;

	vs_in.Pos.w = 1.0f;
	vs_out.Pos = mul(vs_in.Pos, model);
	vs_out.Pos = mul(vs_out.Pos, view);
	vs_out.Pos = mul(vs_out.Pos, proj);

	vs_out.Tex = vs_in.Tex;

	//The projected reflection position;
	vs_out.ReflectPos = mul(vs_in.Pos, model);
	vs_out.ReflectPos = mul(vs_out.ReflectPos, reflection);
	vs_out.ReflectPos = mul(vs_out.ReflectPos, proj);

   return vs_out;	
}