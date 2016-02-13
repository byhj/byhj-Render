#version 430 core

in VS_OUT {
  vec4 color;
}vs_out;

uniform sampler2D tex;

layout (location = 0) out vec4 fragColor;

void main(void)
{
   fragColor = vs_out.color * texture(tex, gl_PointCoord);
}