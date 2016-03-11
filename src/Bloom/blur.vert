#version 430 core

layout (location = 0) in vec4 g_position;
layout (location = 1) in vec2 g_texcoord;

out VS_OUT {
  vec2 texcoord;
}vs_out;

void main(void)
{
   vs_out.texcoord = g_texcoord;
   gl_Position = g_position;
}