#version 430 core

layout (location = 0) out vec4 color;

in GS_OUT
{
    vec4 color;
    vec3 normal;
} fs_in;



void main(void)
{
    color = vec4(abs(fs_in.normal.z)) * fs_in.color;
}
