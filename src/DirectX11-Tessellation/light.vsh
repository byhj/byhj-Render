
struct VS_IN
{
	 float4 Pos       :POSITION;
	 float4 Color     :TEXCOORD;
};

struct VS_OUT
{
    float4 Pos    : POSITION;
    float4 Color  : TEXCOORD;
};

VS_OUT VS(VS_IN vs_in)
{	
 
   VS_OUT vs_out;
   vs_out.Pos = vs_in.Pos;
   vs_out.Color = vs_in.Color;

   return vs_out;
}
