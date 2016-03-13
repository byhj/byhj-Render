#version 420 core

layout (location = 0) in vec4 g_position;
layout (location = 1) in vec3 g_normal;

out VS_OUT
{
    vec4 color;
    vec3 normal;
} vs_out;

void main(void)
{
    vs_out.normal = normal;
    gl_Position = position;

}
