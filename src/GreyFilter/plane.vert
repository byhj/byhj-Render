#version 430 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoord;

out VS_OUT {
  vec2 tc;
}vs_out;

void main(void)
{
   gl_Position = vec4(Position, 1.0);
   vs_out.tc = TexCoord;
}