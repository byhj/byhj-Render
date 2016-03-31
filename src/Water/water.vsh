
cbuffer MatrixObject : register(b0)
{
	float4x4 model;
	float4x4 view;
	float4x4 proj;
    float4x4 reflectMat;
};

struct VS_IN
{
	 float4 Pos       :POSITION;
	 float2 Tex       :TEXCOORD0;
	 float3 Normal    :NORMAL;
};

struct VS_OUT
{
    float4 Pos        : SV_POSITION;
    float2 Tex        : TEXCOORD0;
	float4 reflectPos : TEXCOORD1;
	float4 refractPos : TEXCOORD2;
};

VS_OUT WaterVS(VS_IN vs_in)
{	
 
   VS_OUT vs_out;

   vs_out.Pos = mul(vs_in.Pos, model);
   vs_out.Pos = mul(vs_out.Pos, view);
   vs_out.Pos = mul(vs_out.Pos, proj);

   vs_out.Tex = vs_in.Tex;
   float4x4 reflectProjWorld = mul(reflectMat, proj);
   reflectProjWorld = mul(model, reflectProjWorld);
   
   vs_out.reflectPos = mul(vs_in.Pos, reflectProjWorld);
   vs_out.refractPos = vs_out.Pos;
  
   return vs_out;
}
