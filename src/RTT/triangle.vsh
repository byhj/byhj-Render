
cbuffer cbMatrix : register(b0)
{
   float4x4 g_World;
   float4x4 g_View;
   float4x4 g_Proj;
};

struct VS_IN
{
   float4 Pos : POSITION0;
   float4 Tex : TEXCOORD0;
};

struct VS_OUT
{
    float4 Pos : SV_POSITION;
    float4 Tex : TEXCOORD0;
};

VS_OUT VS( VS_IN vs_in )
{	
 
   VS_OUT vs_out;
   vs_out.Pos.w = 1.0f;
   vs_out.Pos = mul(vs_in.Pos,  g_World);
   vs_out.Pos = mul(vs_out.Pos, g_View);
   vs_out.Pos = mul(vs_out.Pos, g_Proj);

   vs_out.Tex = vs_in.Tex;
 
   return vs_out;
}
