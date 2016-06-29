#version 430 core

layout (location = 0) in vec4 g_position;

void main(void) 
{ 
  gl_Position = vec4(g_position); 
}