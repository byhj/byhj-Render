#version 430 core

layout (location = 0) out vec4  FragColor;

in GE_OUT
{
  vec3 FacetNormal;
  vec3 PatchDistance;
  vec3 TriDistance;

}ge_out;

uniform vec3 lightDir;
uniform vec4 diffuseMat;
uniform vec4 ambientMat;

float amplify(float d, float scale, float offset)
{
  d = scale * d + offset;
  d = clamp(d, 0, 1);
  d = 1 - exp2(-2 * d * d);
  
  return d;
}


void main(void)
{
  vec3 N = normalize(ge_out.FacetNormal);
  vec3 L = lightDir;
  float df = max(0.0f, dot(N, L) );
 // vec3 viewDir = camPos - vec3(gl_FragCoord.x / 1000.0f, gl_FragCoord.y / 600.0f, gl_FragCoord.z);
 // vec3 R = reflect(-L, N);
  //float sp = max(0.0f, dot(viewDir, R));

  vec4 color = ambientMat + df * diffuseMat;// + pow(sp, 32) * diffuseMat;

  float d1 = min(min(ge_out.TriDistance.x, ge_out.TriDistance.y), ge_out.TriDistance.z);
  float d2 = min(min(ge_out.PatchDistance.x, ge_out.PatchDistance.y), ge_out.PatchDistance.z);

   color = amplify(d1, 40, -0.5) * amplify(d2, 60, -0.5) * color;

   FragColor = color;
}