#version 430 core

layout (location = 0) out vec4 g_fragColor;

in VS_OUT {
  vec2 texcoord;
}vs_out;

uniform sampler2D u_sceneTex;
uniform sampler2D u_bloomTex;

uniform bool  enableBloom = true;
uniform float exposure = 1.0f;
uniform sampler2D text;

void main()
{
  const float gamma = 2.2f;
  vec3 hdrColor =  texture2D( u_sceneTex, vs_out.texcoord).rgb;
  vec3 bloomColor = texture2D(u_bloomTex, vs_out.texcoord).rgb;
  
  //If enable bloom, we and the scene color with bloom color
  if (enableBloom) {
    // hdrColor += bloomColor;
  }

  //Tone mapping 
  vec3 color = vec3(1.0f) - exp(-bloomColor * exposure);
  color = pow(color, vec3(1.0f / gamma) );
  color += hdrColor;

  g_fragColor = vec4(color, 1.0f);
}