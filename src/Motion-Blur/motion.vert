#version 430 core

layout (location = 0) in vec3 g_Position;
layout (location = 1) in vec3 g_Normal;
layout (location = 2) in vec2 g_TexCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;
uniform mat4 u_prevMVP;

out VS_OUT
{
   vec4 ClipSpacePos;
   vec3 Normal;
   vec2 TexCoord;
   vec4 prevClipSpacePos;
}vs_out;

void main(void)
{
   mat4 mvp = u_Proj * u_View * u_Model;
   vs_out.ClipSpacePos   = mvp * vec4(g_Position, 1.0f);
   vs_out.Normal         = normalize(g_Normal);
   vs_out.TexCoord       = g_TexCoord;
   vs_out.prevClipSpacePos = u_prevMVP * vec4(g_Position, 1.0f);

   gl_Position = mvp * vec4(g_Position, 1.0f);
}