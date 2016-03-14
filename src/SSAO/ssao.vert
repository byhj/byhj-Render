#version 330 core

layout (location = 0) in vec3 g_position;
layout (location = 1) in vec2 g_texcoord;

out VS_OUT {
  vec2 texcoord;
}vs_out;

void main()
{
    gl_Position = vec4(g_position, 1.0f);
    vs_out.texcoord = g_texcoord;
}