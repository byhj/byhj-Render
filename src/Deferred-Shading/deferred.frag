#version 430 core

layout (location = 0) out vec3 g_PosTex;
layout (location = 1) out vec3 g_NormalTex;
layout (location = 2) out vec3 g_ColorTex;

in VS_OUT 
{
   vec3 FragPos;
   vec3 Normal;
   vec2 TexCoord;
}vs_out;


uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
 
void main(void)
{
   g_PosTex = vs_out.FragPos;
   g_NormalTex = normalize(vs_out.Normal);
   g_ColorTex = vec3(abs(vs_out.Normal.z) );
}
