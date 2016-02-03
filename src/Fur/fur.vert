#version 430 core

layout (location = 0) in vec3 g_Positoin;
layout (location = 1) in vec3 g_Normal;
layout (location = 2) in vec2 g_Texcoord;

out VS_OUT {
  vec3 normal;
  vec2 texcoord;
}vs_out;

uniform mat4 g_Model;
uniform mat4 g_View;
uniform mat4 g_Proj;


void main(void)
{
   mat4 mvp = g_Proj * g_View * g_Model;
   vs_out.normal = inverse( transpose( mat3(g_View * g_Model) ) ) * g_Normal;
   vs_out.texcoord = g_Texcoord;

   gl_Position = mvp * vec4(g_Positoin, 1.0f);
}