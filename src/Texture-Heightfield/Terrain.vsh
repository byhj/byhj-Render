
cbuffer d3d::MatrixBuffer : register(b0)
{
	float4x4 model;
	float4x4 view;
	float4x4 proj;
};

 struct VS_IN
{
    float4 Pos    : POSITION;
    float3 Normal : NORMAL;
	float2 Tex    : TEXCOORD;
};

struct VS_OUT
{
    float4 Pos   : SV_POSITION;
    float3 Normal : NORMAL;
	float2 Tex    : TEXCOORD;
};

VS_OUT TerrainVS( VS_IN vs_in)
{	
 
   VS_OUT vs_out;
   vs_in.Pos.w = 1.0f;
   vs_out.Pos = mul(vs_in.Pos,  model);
   vs_out.Pos = mul(vs_out.Pos, view);
   vs_out.Pos = mul(vs_out.Pos, proj);

   vs_out.Normal = mul(vs_in.Normal, (float3x3)model);
   vs_out.Tex    = vs_in.Tex * 15.0f;
 
   return vs_out;
}
