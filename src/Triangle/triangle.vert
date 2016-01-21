#version 430 core

layout (location = 0) in vec4 g_Position;
layout (location = 1) in vec4 g_Color;

out VS_OUT {
  vec4 color;
};

void main()
{
   VS_OUT  vs_out;
   vs_out.color = g_Color;
   gl_Position = g_Position;


}