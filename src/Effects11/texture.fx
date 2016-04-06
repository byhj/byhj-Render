#include "lightHelper.fx"

cbuffer MatrixBuffer : register(b0)
{
	float4x4 g_World;
	float4x4 g_View;
	float4x4 g_Proj;
};

 struct VS_IN
{
    float4 Pos      : POSITION;
    float3 Normal   : NORMAL;
	float2 Texcoord : TEXCOORD;
};

struct VS_OUT
{
    float4 Pos      : SV_POSITION;
	float3 Normal   : NORMAL;
	float2 Texcoord : TEXCOORD0;
};

Texture2D g_DiffuseMap;

SamplerState samAnisotropic
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;

	AddressU = WRAP;
	AddressV = WRAP;
};

VS_OUT VS( VS_IN vs_in)
{	
 
   VS_OUT vs_out;
   vs_in.Pos.w = 1.0f;
   vs_out.Pos = mul(vs_in.Pos,  g_World);
   vs_out.Pos = mul(vs_out.Pos, g_View);
   vs_out.Pos = mul(vs_out.Pos, g_Proj);
   vs_out.Pos = vs_in.Pos;
   vs_out.Texcoord = vs_in.Texcoord;
   vs_out.Normal   = mul(vs_in.Normal, (float3x3)g_World );

   return vs_out;
}



float4 PS( VS_OUT ps_in) : SV_Target
{    
   /*
    ps_in.Normal = normalize(ps_in.Normal);
    float3 eyeDir = normalize( (float3)g_EyePos - ps_in.WorldPos);
  
	float4 ambient  = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse  = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Sum the light contribution from each light source.
	for (int i = 0; i != 3; ++i)
	{
	   float4 A, D, S;
	   ComputeDirectionalLight(g_Mat, g_DirLights[i], ps_in.Normal, eyeDir, A, D, S);
	   ambient   += A;  
	   diffuse   += D;
	   specular  += S;
	}


	float4 lightColor = ambient + diffuse + specular + texColor;

	// Common to take alpha from diffuse material.
	lightColor.a = g_Mat.Diffuse.a;

	return lightColor;
	*/
	float4 texColor	 = g_DiffuseMap.Sample(samAnisotropic, ps_in.Texcoord );
	return texColor	;
}

technique11 LightTech1
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL);
        SetPixelShader( CompileShader( ps_5_0, PS() ) );
    }
}

technique11 LightTech2
{
   pass p0
   {
     SetVertexShader( CompileShader( vs_5_0, VS() ) );
	 SetGeometryShader( NULL);
	 SetPixelShader( CompileShader(ps_5_0, PS() ) );
   }
}

technique11 LightTech3
{
  pass p0
  {
     SetVertexShader( CompileShader(vs_5_0, VS() ) );
	 SetGeometryShader(NULL);
	 SetPixelShader( CompileShader(ps_5_0, PS() ) ); 
  }
}