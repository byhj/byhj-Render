#version 430 core

layout (location = 0) out vec4 g_fragColor;

void main(void)
{
   g_fragColor = vec4(vec3(gl_FragCoord.z), 1.0f);
}