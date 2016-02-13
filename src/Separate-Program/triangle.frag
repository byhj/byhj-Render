#version 430 core

layout (location = 0) out vec4 fragColor;

uniform vec4 color;

void main(void)
{
   fragColor = color;
}