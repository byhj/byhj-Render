

#version 430 core

layout (location = 0) out vec4 g_FragColor;

in VS_OUT {
  vec3 normal;
  vec2 texcoord;
}vs_out;

uniform sampler2D box_tex;
uniform float fogStart = -10.0f;
uniform float fogEnd = 10.0f;
uniform vec4  fogColor = vec4(0.5f, 0.5f, 0.5f, 0.5f);
uniform vec3  camPos = vec3(0.0f, 0.0f, 3.0f);

void main(void)
{  
   float fogFactor = (fogEnd - camPos.z) / (fogEnd - fogStart);
   vec4 texColor = texture2D(box_tex, vs_out.texcoord); 

   //blend the object color with fog color make the fog effect
   g_FragColor = fogFactor * texColor + (1.0f - fogFactor) * fogColor;

}