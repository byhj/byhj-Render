#version 430 core

layout (location = 0) out vec4 g_FragColor;

in VS_OUT {
  vec3 normal;
  vec2 texcoord;
}vs_out;

void main()
{
//abs(vs_out.normal.z) 
   g_FragColor = vec4(1.0f);
}