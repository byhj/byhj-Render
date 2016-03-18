#version 430 core

layout (location = 0) in vec3 g_Position;
layout (location = 1) in vec3 g_Normal;
layout (location = 2) in vec2 g_Texcoord;

out VS_OUT {
   vec3 normal;
   vec2 texcoord;
}vs_out;

uniform mat4 u_mvp;

void main(void)
{
   vs_out.normal = normalize(g_Normal);
   vs_out.texcoord = g_Texcoord;

   gl_Position = u_mvp * vec4(g_Position, 1.0f);
}