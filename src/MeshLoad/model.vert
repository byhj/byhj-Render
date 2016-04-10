#version 430 core

layout (location = 0) in vec3 g_position;
layout (location = 1) in vec3 g_normal;
layout (location = 2) in vec2 g_texcoord;
layout (location = 3) in vec3 g_tangent;
layout (location = 4) in vec3 g_bitangent;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

out VS_OUT
{
  vec2 texcoord;
  vec3 fragPos;
  vec3 tangentLightPos;
  vec3 tangentViewPos;
  vec3 tangentFragPos;
}vs_out;

uniform vec3 u_lightPos = vec3(0.0f,  20.0f, 15.0f);
uniform vec3 u_camPos  = vec3(0.0f, 10.0f, 15.0f);

void main(void)
{
   mat3 normalMat = transpose(inverse(mat3(u_view * u_model)));
   vec3 T = normalize(normalMat * g_tangent);
   vec3 B = normalize(normalMat * g_bitangent);
   vec3 N = normalize(normalMat * g_normal);   
   mat3 TBN = transpose(mat3(T, B, N));  

   vs_out.texcoord  = g_texcoord;
   vs_out.fragPos  =  mat3(u_model) * g_position;
   vs_out.tangentLightPos = TBN * u_lightPos;
   vs_out.tangentViewPos  = TBN * u_camPos;
   vs_out.tangentFragPos  = TBN * g_position;

   mat4 mvp = u_proj * u_view * u_model;
   gl_Position = mvp * vec4(g_position, 1.0f);
}