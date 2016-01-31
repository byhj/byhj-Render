#version 430 core

layout (location = 0) in vec3 g_Position;

out VS_OUT {
  vec3 texcoord;
}vs_out;

uniform mat4 g_model;
uniform mat4 g_view;
uniform mat4 g_proj;

void main()
{
   mat4 mvp = g_proj * g_view * g_model;
   vs_out.texcoord = g_Position;
   gl_Position  = mvp * vec4(g_Position, 1.0f);

}