#version 430 core

layout (location = 0) in vec3 g_Position;
uniform mat4 mvp;

//Your should declar it
out gl_PerVertex
{
	vec4 gl_Position;
};

void main(void)
{
    gl_Position = vec4(g_Position, 1.0f);
}