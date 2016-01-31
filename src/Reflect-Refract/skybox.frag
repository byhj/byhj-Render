#version 430 core

in VS_OUT {
  vec3 texcoord;
}vs_out;

layout (location = 0) out vec4 fragColor;
uniform samplerCube skybox;

void main(void)
{
   fragColor = texture(skybox, vs_out.texcoord);
}