#version 430 core

layout (location = 0) out vec4 g_fragColor;

in VS_OUT {
  vec2 texcoord;
}vs_out;


uniform sampler2D u_tex;

void main(void)
{
   vec4 texColor = texture(u_tex, vs_out.texcoord);
   g_fragColor = texColor;
}
