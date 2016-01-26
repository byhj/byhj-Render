#version 430 core

in VS_OUT {
  vec4 color;
}vs_out;

layout (location = 0) out vec4 fragColor;

void main(void)
{
   fragColor = vs_out.color;
}