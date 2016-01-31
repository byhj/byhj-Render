#version 430 core

layout (location = 0) out vec4 g_FragColor;

in VS_OUT 
{
   vec3 FragPos;
   vec3 Normal;
   vec2 TexCoord;
}vs_out;

uniform samplerCube skybox;
uniform vec3 u_CameraPos = vec3(0.0, 1.0f, 3.0f);
 
void main(void)
{
    float ratio = 1.00 / 1.52;
    vec3 I = normalize(vs_out.FragPos - u_CameraPos);
    vec3 R = refract(I, normalize(vs_out.Normal), ratio);

    g_FragColor = texture(skybox, R);
}
