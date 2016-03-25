#version 430 core

layout (location = 0) out vec4 g_fragColor;

in VS_OUT {
   vec3 normal;
   vec3 worldPos;
}vs_out;

uniform samplerCube u_skybox;
uniform vec3 u_camPos = vec3(0.0f, 1.0f, 0.0f);

void main(void)
{
   float ratio = 1.0 / 1.52;
   vec3  I = normalize(vs_out.worldPos - u_camPos);
   vec3  R = refract(I, normalize(vs_out.normal), ratio);
   g_fragColor = texture(u_skybox, R);
}