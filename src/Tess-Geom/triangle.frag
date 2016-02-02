#version 430 core

in GEOM_OUT {
  vec4 color;
}geom_out;

layout (location = 0) out vec4 g_FragColor;

void main(void)
{
   g_FragColor = geom_out.color;
}