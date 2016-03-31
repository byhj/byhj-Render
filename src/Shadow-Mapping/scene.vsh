
cbuffer d3d::MatrixBuffer : register(b0)
{
	float4x4 model;
	float4x4 view;
	float4x4 proj;
	float4x4 lightView;
	float4x4 lightProj;
};

cbuffer LightBuffer : register(b1)
{
  float4 LightPos;
};

struct VS_IN
{
	 float4 Pos       :POSITION;
	 float2 Tex       :TEXCOORD0;
	 float3 Normal    :NORMAL;
};

struct VS_OUT
{
    float4 Pos         : SV_POSITION;
    float2 Tex         : TEXCOORD0;
	float3 Normal      : NORMAL;
	float4 LightViewPos: TEXCOORD1;
	float3 LightPos    : TEXCOORD2;
};

VS_OUT VS(VS_IN vs_in)
{	
 
   VS_OUT vs_out;
   vs_out.Pos.w = 1.0f;

  // Make the position with mvp matrix
   vs_out.Pos = mul(vs_in.Pos, model);
   vs_out.Pos = mul(vs_out.Pos, view);
   vs_out.Pos = mul(vs_out.Pos, proj);

   vs_out.LightViewPos = mul(vs_in.Pos, model);
   vs_out.LightViewPos = mul(vs_out.LightViewPos, lightView);
   vs_out.LightViewPos = mul(vs_out.LightViewPos, lightProj);

   vs_out.Tex = vs_in.Tex;

   vs_out.Normal = mul(vs_in.Normal, (float3x3)model);
   vs_out.Normal = normalize(vs_out.Normal);

   float4 worldPos = mul(vs_in.Pos, model);
   vs_out.LightPos = LightPos.xyz - worldPos.xyz;
   vs_out.LightPos = normalize(vs_out.LightPos);

   return vs_out;
}
