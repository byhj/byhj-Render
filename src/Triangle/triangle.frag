#version 430 core

in VS_OUT {
  vec4 color;
};

layout (location = 0) out vec4 fragColor;

void main(void)
{
   vs_out vs_out;
   
   fragColor = vs_out.color;
}