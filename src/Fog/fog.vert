#version 430 core

layout (location = 0) in vec3 g_Position;
layout (location = 1) in vec3 g_Normal;
layout (location = 2) in vec2 g_Texcoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

out VS_OUT {
   vec3 normal;
   vec2 texcoord;
}vs_out;

void main(void)
{
   mat4 mvp = u_proj * u_view * u_model;
   vs_out.normal = g_Normal;
   vs_out.texcoord = g_Texcoord;
   gl_Position  = mvp * vec4(g_Position, 1.0f);

   
}