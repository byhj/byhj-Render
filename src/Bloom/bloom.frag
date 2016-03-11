#version 430 core

layout (location = 0) out vec4 g_fragColor;
layout (location = 1) out vec4 g_lightColor;

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

    vec3 result =  diffuseColor.rgb + 2.0f * glowColor.rgb;
	    // Check whether result is higher than some threshold, if so, output as bloom threshold color
    float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0) {
        g_lightColor = vec4(result, 1.0);
    }

	g_fragColor = vec4(result, 1.0f);
}