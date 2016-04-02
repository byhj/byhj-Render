#version 430 core

layout (location = 0) out vec4 fragColor;

in VS_OUT
{
 vec3 normal;
 vec3 lightDir;
 vec3 viewDir;
}vs_out;


uniform vec3 diffuse_material = vec3(0.5, 0.2, 0.7);
uniform float power = 32.0f;
uniform vec3 specular_material = vec3(0.6);

void main(void)
{
    vec3 ambient = vec3(0.1f);
    vec3 N = normalize(vs_out.normal);
	vec3 L = normalize(vs_out.lightDir);
	vec3 V = normalize(vs_out.viewDir);

	vec3 R = reflect(-L, N);
	vec3 diffuse = max(dot(N, L), 0.0) * diffuse_material;
	vec3 specular = pow(max(dot(R, V), 0.0), power) * specular_material;

    vec3 result = (ambient + specular + diffuse);
	fragColor = vec4(result, 1.0f);
}