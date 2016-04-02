#version 430 core

layout (location = 0) out float g_fragColor;

in VS_OUT {
  vec2 texcoord;
}vs_out;

uniform sampler2D u_ssaoTex;
const int blurSize = 4; // use size of noise texture (4x4)

void main() 
{
   vec2 texelSize = 1.0 / vec2(textureSize(u_ssaoTex, 0));
   float result = 0.0;
   for (int x = 0; x < blurSize; ++x) 
   {
      for (int y = 0; y < blurSize; ++y) 
      {
         vec2 offset = (vec2(-2.0) + vec2(float(x), float(y))) * texelSize;
         result += texture(u_ssaoTex, vs_out.texcoord + offset).r;
      }
   }
 
   g_fragColor = result / float(blurSize * blurSize);
}