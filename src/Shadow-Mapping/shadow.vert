 #version 430 core

uniform mat4 u_model;
uniform mat4 u_lightMat;

layout (location = 0) in vec4 position;

void main(void)
{   
    gl_Position = u_lightMat * u_model * position;
}
