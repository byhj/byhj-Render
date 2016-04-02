#version 410

layout (location = 0) in vec4 g_position;
layout (location = 1) in vec3 g_normal;
layout (location = 2) in vec2 g_texcoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

out VS_OUT
{
    vec3 normal;
}vs_out;

void main(void)
{
    vs_out.normal = mat3(transpose(inverse(u_view * u_model))) * g_normal;
    gl_Position = (u_proj * u_view * u_model)  * g_position;
};