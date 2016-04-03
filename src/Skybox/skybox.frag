#version 430 core

layout (location = 0) out vec4 fragColor;

in VS_OUT {
  vec3 texcoord;
}vs_out;


uniform samplerCube u_skybox;

void main(void)
{
   fragColor = texture(u_skybox, vs_out.texcoord);
}