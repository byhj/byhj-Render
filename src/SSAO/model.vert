#version 330 core

layout (location = 0) in vec3 g_position;
layout (location = 1) in vec3 g_normal;
layout (location = 2) in vec2 g_texcoord;

out VS_OUT  {
   vec3 fragPos;
   vec3 normal;
   vec2 texcoord;
}vs_out;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

void main()
{
    vec4 viewPos = u_view * u_model * vec4(g_position, 1.0f);
    vs_out.fragPos  = viewPos.xyz; 
    vs_out.texcoord = g_texcoord;
    
    mat3 normalMat = transpose(inverse(mat3(u_view * u_model)));
    vs_out.normal = normalMat * g_normal;

	gl_Position = u_proj * viewPos;
}