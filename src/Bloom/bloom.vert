#version 430 core

layout (location = 0) in vec4 g_position;
layout (location = 1) in vec3 g_normal;
layout (location = 2) in vec2 g_texcoord;

out VS_OUT  {
   vec3 fragPos;
   vec3 normal;
   vec2 texcoord;
}vs_out;


uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;


void main()
{
   mat4 mvp = u_proj * u_view * u_model;
   vs_out.normal = mat3( transpose(inverse(u_model)) ) * g_normal;
   vs_out.texcoord = g_texcoord * 5.0f;
   gl_Position = mvp * g_position;
}