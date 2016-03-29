#version 430 core

layout (location = 0) in vec3 g_position;
layout (location = 1) in vec2 g_texcoord;

out VS_OUT {
  vec2 texcoord;
}vs_out;

uniform mat4 u_ortho;

void main()
{
   vs_out.texcoord = g_texcoord;
   gl_Position = u_ortho * vec4(g_position, 1.0f);
}