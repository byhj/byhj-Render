#version 430 core

layout (location = 0) out vec4 g_FragColor;


in VS_OUT {
  vec2 texcoord;
}vs_out;

uniform sampler2D tex;

void main(void)
{
   vec4 texColor = texture2D(tex, vs_out.texcoord);
   g_FragColor = texColor;
}