#version 430 core

layout (location = 0) in vec4 g_Position;

uniform mat4 u_mvp;

void main(void)
{
    gl_Position = u_mvp * g_Position;
}
