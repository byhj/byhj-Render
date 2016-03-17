 
 struct MatrixType
 {
    float4x4 model;
	float4x4 view;
	float4x4 proj;
 };

cbuffer MatrixBuffer :register(b0)
{
	MatrixType g_Mat;
};

struct VS_IN
{
    float4 Pos    : POSITION; //POSITION is same as POSITION0
    float2 Tex    : TEXCOORD0;
	float3 Normal : NORMAL;
};

struct VS_OUT
{
    float4 Pos      : SV_POSITION;
    float2 Tex      : TEXCOORD0;
	float3 Normal   : NORMAL;
	float3 worldPos : POSITION;
};

VS_OUT VS( VS_IN vs_in )
{	

   VS_OUT vs_out;
   float4x4 mvp;
   mvp = mul(g_Mat.model, g_Mat.view);
   mvp = mul(mvp, g_Mat.proj);

   vs_in.Pos.w = 1.0f;
   vs_out.Pos = mul(vs_in.Pos, mvp);
   vs_out.Normal = mul(g_Mat.model, vs_in.Normal );
   vs_out.Tex = vs_in.Tex;
   vs_out.worldPos = mul(g_Mat.model, vs_in.Pos);

   return vs_out;
}
