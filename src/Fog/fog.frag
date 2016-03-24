

#version 430 core

layout (location = 0) out vec4 g_fragColor;

in VS_OUT {
  vec3 viewPos;
  vec3 normal;
  vec2 texcoord;
}vs_out;



uniform sampler2D box_tex;
uniform float  u_fogStart = -1.0f;
uniform float  u_fogEnd = 1.0f;
uniform vec4   u_fogColor = vec4(0.5f, 0.5f, 0.5f, 0.5f);
uniform float  u_fogDensity = 1.0f;
uniform vec3   u_camPos = vec3(0.0f, 0.0f, 3.0f);

uniform int   u_fogMode = 0; //0:linear, 1:exponential, 2:exponential square
uniform int   u_baseMode = 0; //0:plane based, 1:range based

void main(void)
{  

   float dist = 0.0f;
   float fogFactor = 0.0f;
   vec4 finalColor;
   vec4 texColor = texture2D(box_tex, vs_out.texcoord);

   if (u_baseMode == 0) {
	 dist = abs(vs_out.viewPos.z);
	 dist = length(vs_out.viewPos);
	 dist = (gl_FragCoord.z / gl_FragCoord.w);
   } else {
	 dist = length(vs_out.viewPos);
   }

   if (u_fogMode == 0) {
      fogFactor = (u_fogEnd - dist) / (u_fogEnd - u_fogStart);
	  fogFactor = clamp(fogFactor, 0.0f, 1.0f);
	  //mix = fogColor * ( 1-fogFactor) + texColor * fogFactor
	  finalColor = mix(u_fogColor, texColor, fogFactor);
   } else if (u_fogMode == 1) {
      fogFactor = 1.0f / exp(dist * u_fogDensity);
	  fogFactor = clamp(fogFactor, 0.0f, 1.0f);
	  finalColor = mix(u_fogColor, texColor, fogFactor);
   } else {
     fogFactor = 1.0f / exp( pow(dist * u_fogDensity, 2) );
	 fogFactor = clamp(fogFactor, 0.0f, 1.0f);
	 finalColor = mix(u_fogColor, texColor, fogFactor);
   }

   /*

   */
   //show fogFactor depth
   //finalColor = vec4(1-fogFactor);
   g_fragColor =  finalColor;

}