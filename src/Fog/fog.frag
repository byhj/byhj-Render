#version 430 core

layout (location = 0) out vec4 g_FragColor;

in VS_OUT {
  vec3 normal;
  vec2 texcoord;
}vs_out;

uniform sampler2D box_tex;

void main(void)
{
   g_FragColor = texture2D(box_tex, vs_out.texcoord); 
}