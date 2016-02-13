

#version 430 core

layout (location = 0) out vec4 g_FragColor;

in VS_OUT {
  vec3 normal;
  vec2 texcoord;
  flat int index;
}vs_out;

uniform sampler2DArray texArray;
uniform int texIndex =1;

void main(void)
{  
   vec4 texColor = texture(texArray, vec3(vs_out.texcoord, vs_out.index) ); 
   g_FragColor = texColor;

}