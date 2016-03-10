
cbuffer MatrixObject : register(b0)
{
	float4x4 model;
	float4x4 view;
	float4x4 proj;

	float4x4 view2;
    float4x4 proj2;
};

struct VS_IN
{
	 float4 Pos       :POSITION;
	 float2 Tex       :TEXCOORD0;
	 float3 Normal    :NORMAL;
};

struct VS_OUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
	float3 Normal: NORMAL;
	float4 viewPos : TEXCOORD1;
};

VS_OUT VS(VS_IN vs_in)
{	
 
   VS_OUT vs_out;

  // Make the position with mvp matrix
   vs_out.Pos = mul(vs_in.Pos, model);
   vs_out.Pos = mul(vs_out.Pos, view);
   vs_out.Pos = mul(vs_out.Pos, proj);

   vs_out.Tex = vs_in.Tex;
   vs_out.Normal = mul(vs_in.Normal, (float3x3)model);
   vs_out.Normal = normalize(vs_out.Normal);

   vs_out.viewPos = mul(vs_in.Pos, model);
   vs_out.viewPos = mul(vs_out.viewPos, view2);
   vs_out.viewPos = mul(vs_out.viewPos, proj2);

   return vs_out;
}
