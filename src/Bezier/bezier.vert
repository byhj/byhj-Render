#version 430 core

layout (location = 0) in vec4 g_Position;

uniform mat4 u_mv;

void main(void)
{
    gl_Position = u_mv * g_Position;
}
