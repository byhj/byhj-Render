#version 430 core

layout (location = 0) in vec4 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoord;

out VS_OUT
{
 vec3 normal;
 vec3 lightDir;
 vec3 viewDir;
}vs_out;

uniform vec3 lightPos = vec3(1.0f, 1.0f, 5.0f);
uniform vec3 camPos = vec3(0.0f, 0.0f, 3.0f);
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main(void)
{
   mat4 mvp = proj * view * model;
   vec4 worldPos =  view * model * Position;

   vs_out.normal = mat3( transpose( inverse(model) ) ) * Normal; 
   vs_out.lightDir = lightPos - worldPos.xyz;
   vs_out.viewDir  = camPos - worldPos.xyz;

   gl_Position = mvp * Position;
   
}