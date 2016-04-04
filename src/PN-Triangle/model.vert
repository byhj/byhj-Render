#version 430 core

layout (location = 0) in vec3 g_position;
layout (location = 1) in vec3 g_normal;
layout (location = 2) in vec2 g_texcoord;

uniform mat4 u_model;

out VS_OUT
{
   vec3 fragPos;
   vec3 normal;
   vec2 texcoord;
}vs_out;

void main(void)
{
   vs_out.fragPos  = mat3(u_model) * g_position;
   vs_out.normal   = normalize( mat3( transpose( inverse(u_model) ) ) * g_normal );
   vs_out.texcoord = g_texcoord;
}