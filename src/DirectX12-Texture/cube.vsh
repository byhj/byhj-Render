
struct VS_IN 
{
  float4 Pos  : POSITION;
  float4 Color: COLOR0;
};

struct VS_OUT
{
  float4 Pos   : SV_POSITION;
  float4 Color : COLOR0;
};

VS_OUT VS_MAIN( VS_IN vs_in)
{
  VS_OUT vs_out;
  vs_out.Pos   = vs_in.Pos;
  vs_out.Color = vs_in.Color;

  return vs_out;
}