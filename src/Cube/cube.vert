#version 430 core

layout (location = 0) in vec3 g_Position;

out VS_OUT {
  vec4 color;
}vs_out;

uniform mat4 g_model;
uniform mat4 g_view;
uniform mat4 g_proj;

void main()
{
   mat4 mvp = g_proj * g_view * g_model;
   vs_out.color = vec4(g_Position + vec3(0.5f), 1.0f);
   //your must use matrix x vector order
   gl_Position  = mvp * vec4(g_Position, 1.0f);

}