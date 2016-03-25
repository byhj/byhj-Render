#version 430 core

in VS_OUT {
  vec3 texcoord;
}vs_out;

layout (location = 0) out vec4 fragColor;
uniform samplerCube u_skybox;

void main(void)
{
   fragColor = texture(u_skybox, vs_out.texcoord);
}