#version 430 core

layout (location = 0) in vec3 Position;

out VS_OUT
{
  vec3 Pos;
}vs_out;

void main(void)
{
   vs_out.Pos = Position;
}
