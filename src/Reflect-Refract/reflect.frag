#version 330

layout (location = 0) out vec4 fragColor;

in vec3 Normal;
in vec3 WorldPos;

uniform samplerCube skybox;
uniform vec3 cameraPos = vec3(0.0f, 1.0f, 3.0f);

void main()
{
    vec3 I = normalize(WorldPos - cameraPos);
    vec3 R = reflect(I, normalize(Normal));

    fragColor = texture(skybox, R);
}