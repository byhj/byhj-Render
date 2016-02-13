#version 430

layout (location = 0) out vec4 g_FragColor;

in VS_OUT {
  vec3 normal;
}vs_out;

void main()
{
    vec3 lightDir = vec3(1.0f, 5.0f, 0.0f);
    float intensity = dot(lightDir, vs_out.normal);

	vec4 color;
	if (intensity > 0.95)
		color = vec4(1.0,0.5,0.5,1.0);
	else if (intensity > 0.5)
		color = vec4(0.6,0.3,0.3,1.0);
	else if (intensity > 0.25)
		color = vec4(0.4,0.2,0.2,1.0);
	else
		color = vec4(0.2,0.1,0.1,1.0);

	g_FragColor = color;
}
