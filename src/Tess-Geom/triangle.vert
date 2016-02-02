#version 430 core

layout (location = 0) in vec3 g_Position;
layout (location = 1) in vec3 g_Color;

uniform vec4 offset;

out VS_OUT 
{
  vec4 color;
}vs_out;

void main(void)
{
    vs_out.color = vec4(g_Color, 1.0f);
	gl_Position  =  vec4(g_Position, 1.0f) + offset;
}
