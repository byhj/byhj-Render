#version 430 core

layout (location = 0) out vec4 g_fragColor;

in VS_OUT  {
  vec3 fragPos;
  vec3 normal;
  vec2 texcoord;
}vs_out;

uniform sampler2D u_diffuseTex;
uniform sampler2D u_glowTex;

void main()
{
    vec3 normal = normalize(vs_out.normal);
	
    vec4 diffuseColor = texture2D(u_diffuseTex, vs_out.texcoord);
	vec4 glowColor = texture2D(u_glowTex, vs_out.texcoord);

	g_fragColor =  diffuseColor + 2.0f * glowColor;
}