

struct VS_IN {
  float4 Pos : POSITION;
  float4 Tex : TEXCOORD;
};

struct VS_OUT {
   float4 Pos : SV_POSITION;
   float4 Tex : TEXCOORD;
};

VS_OUT RTT_VS(VS_IN vs_in)
{
   VS_OUT vs_out;
   vs_out.Pos = vs_in.Pos;;
   vs_out.Pos.w = 1.0f;

   vs_out.Tex = vs_in.Tex;

   return vs_out;
}
