#version 430 core

layout (location = 0) in vec3 g_Position;
layout (location = 1) in vec3 g_Normal;
layout (location = 2) in vec2 g_Texcoord;

out VS_OUT {
  vec3 normal;
}vs_out;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

void main(void)
{
   mat4 mv  = u_View * u_Model;
   mat4 mvp = u_Proj * mv;
   vs_out.normal = mat3( transpose( inverse(mv) ) ) * g_Normal;
   gl_Position = mvp * vec4(g_Position, 1.0f);
}