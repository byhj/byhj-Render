#version 430 core

layout (location = 0) out vec2 g_MotionVec;
layout (location = 1) out vec3 g_ColorTex;


in VS_OUT 
{
   vec4 ClipSpacePos;
   vec3 Normal;
   vec2 TexCoord;
   vec4 prevClipSpacePos;
}vs_out;


uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
 
void main(void)
{
   vec3 ndcPos = (vs_out.ClipSpacePos / vs_out.ClipSpacePos.w).xyz;
   vec3 prevNdcPos = (vs_out.prevClipSpacePos / vs_out.prevClipSpacePos.w).xyz;
 
   g_MotionVec = (ndcPos - prevNdcPos).xy;
   g_ColorTex = texture2D(texture_diffuse, vs_out.TexCoord).xyz;
}
