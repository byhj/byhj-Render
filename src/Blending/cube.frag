#version 430 core

in VS_OUT {
  vec4 color0;
  vec4 color1;
} vs_out;

layout (location = 0, index = 0) out vec4 color0; //Ë«Ô´»ìºÏ
layout (location = 0, index = 1) out vec4 color1;

void main(void)
{
   color0 = vs_out.color0;
   color1 = vs_out.color1;
}