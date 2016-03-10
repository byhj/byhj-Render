#version 430 core

layout (location = 0) in vec3 g_Position;
layout (location = 1) in vec3 g_Normal;
layout (location = 2) in vec2 g_Texcoord;
layout (location = 3) in vec3 g_Tangent;
layout (location = 4) in vec3 g_Bitangent;

out VS_OUT {
  vec2 texcoord;
  vec3 lightPos;
  vec3 viewPos;
  vec3 fragPos;
}vs_out;

uniform mat4 g_model;
uniform mat4 g_view;
uniform mat4 g_proj;

uniform vec3 lightPos = vec3(0.0f, 0.0f, 2.0f);
uniform vec3 viewPos  = vec3(0.0f, 1.0f, 5.0f);

void main()
{
   mat4 mvp = g_proj * g_view * g_model;

    mat3 normalMatrix = transpose(inverse(mat3(g_model)));
    vec3 T = normalize(normalMatrix * g_Tangent);
    vec3 B = normalize(normalMatrix * g_Bitangent);
    vec3 N = normalize(normalMatrix * g_Normal);   
	mat3 TBN = transpose(mat3(T, B, N));  

    vs_out.lightPos = TBN * lightPos;
    vs_out.viewPos  = TBN * viewPos;
    vs_out.fragPos  = TBN * g_Position;
	vs_out.texcoord = g_Texcoord;

    //your must use matrix x vector order
    gl_Position  = mvp * vec4(g_Position, 1.0f);

}