#version 430 core

layout(location = 0) out vec4 g_FragColor;

in GEOM_OUT {
   vec3 normal;
   vec2 texcoord;
}geom_out;

void main(void)
{
   g_FragColor = vec4(0.5f, 0.5, 0.5, 1.0f);
}