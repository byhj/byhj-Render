#version 430 core

layout (location = 0) in vec3 g_Position;
layout (location = 1) in vec2 g_Texcoord;

out VS_OUT {
  vec2 texcoord;
}vs_out;

void main(void)
{
   vs_out.texcoord = g_Texcoord;
   gl_Position = vec4(g_Position, 1.0f);
}
