#version 430 core

layout (location = 0) out vec4 g_fragColor;

uniform sampler2D motionVec;
uniform sampler2D colorTex;


uniform vec3 u_viewPos = vec3(0.0f, 0.0f, 5.0f);
uniform vec3 u_lightPos = vec3(0.0f, 1.0f, 3.0f);

in VS_OUT 
{
   vec2 TexCoord;
}vs_out;

const int nSamples = 16;

void main(void)
{
    vec2 motion  = texture(motionVec, vs_out.TexCoord).xy;
	vec4 result = texture(colorTex, vs_out.TexCoord);

   for (int i = 1; i < nSamples; ++i) {
   // get offset in range [-0.5, 0.5]:
      vec2 offset = motion * (float(i) / float(nSamples - 1) - 0.5);
   // sample & add to result:
      result += texture(colorTex, vs_out.TexCoord + offset);
   }
 
   result /= float(nSamples);

    g_fragColor = result;
}