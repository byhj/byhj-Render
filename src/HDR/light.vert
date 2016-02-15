#version 430 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoord;

out VS_OUT 
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
} vs_out;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

uniform bool inverse_normals = true;

void main()
{
    gl_Position = proj* view * model * vec4(Position, 1.0f);
    vs_out.FragPos = vec3(model * vec4(Position, 1.0));   
    vs_out.TexCoord = TexCoord;
    
    vec3 n = inverse_normals ? -Normal : Normal;
    
    mat3 normalMatrix = transpose( inverse(mat3(model)) );
    vs_out.Normal = normalize(normalMatrix * n);
}