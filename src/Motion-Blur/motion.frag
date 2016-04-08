#version 430 core

layout (location = 0) out vec2 g_MotionVec;
layout (location = 1) out vec3 g_ColorTex;


in VS_OUT 
{
   smooth vec4 ClipSpacePos;
   vec3 Normal;
   vec2 TexCoord;
   smooth vec4 prevClipSpacePos;
}vs_out;


uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
 
void main(void)
{
   vec3 ndcPos = (vs_out.ClipSpacePos / vs_out.ClipSpacePos.w).xyz;
   vec3 prevNdcPos = (vs_out.prevClipSpacePos / vs_out.prevClipSpacePos.w).xyz;
   ndcPos = ndcPos * 0.5f + 0.5f;
   prevNdcPos =  prevNdcPos * 0.5f + 0.5f;

   g_MotionVec = (ndcPos - prevNdcPos).xy;
   g_ColorTex = texture2D(texture_diffuse, vs_out.TexCoord).xyz;
}
