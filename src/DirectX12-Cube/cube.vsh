
cbuffer cbMatrix : register(b0)
{
  float4x4 MVP;
}

struct VS_IN {
   float3 Pos   : POSITION;
   float4 Color : COLOR0;
}

struct VS_OUT {
   float4 Pos   : SV_POSITION;
   float4 Color : COLOR0;
}

VS_OUT CUBE_VS(VS_IN vs_in) 
{
  VS_OUT vs_out;
  vs_out.Pos = mul(float4(vs_in.Pos, 1.0f), MVP);
  vs_out.Color = vs_in.Color;

  return vs_out;
}

