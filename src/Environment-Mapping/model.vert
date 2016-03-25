#version 430 core

layout (location = 0) in vec3 g_position;
layout (location = 1) in vec3 g_normal;
layout (location = 2) in vec2 g_texcoord;

out VS_OUT {
  vec3 normal;
  vec3 worldPos;
}vs_out;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

void main(void)
{
  mat4 mvp = u_proj * u_view * u_model;
  vs_out.normal = mat3(transpose(inverse(u_model))) * g_normal;
  vs_out.worldPos = vec3(u_model * vec4(g_position, 1.0f));
  gl_Position = mvp * vec4(g_position, 1.0f);

}