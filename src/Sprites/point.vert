#version 430 core
#extension GL_NV_shader_buffer_load : enable

layout (location = 0) in vec3 g_Position;
layout (location = 1) in vec3 g_Color;

out VS_OUT {
  vec4 color;
}vs_out;

uniform float time;
uniform mat4  g_proj;


void main()
{
    vec3 newVertex = g_Position;  
    newVertex.z += time;                                      
    newVertex.z = fract(newVertex.z);                                                                           
    float size = (20.0 * newVertex.z * newVertex.z);  
	vs_out.color = vec4(smoothstep(1.0, 7.0, size) * g_Color, 1.0f) ;
	  
    newVertex.z = (999.9 * newVertex.z) - 1000.0;             
    gl_Position = g_proj * vec4(newVertex, 1.0f);                    
    gl_PointSize = size; 
}