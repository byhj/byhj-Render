#version 430 core

layout (location = 0) out vec4 g_FragColor;

uniform vec4 u_color = vec4(0.5, 0.8, 1.0, 1.0);

void main(void)
{
    g_FragColor = u_color;
}
