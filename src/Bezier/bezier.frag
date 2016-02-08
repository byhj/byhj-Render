#version 430 core

layout (location = 0) out vec4 g_FragColor;

in TES_OUT
{
    vec3 normal;
} tes_out;

void main(void)
{
    vec3 normal = normalize(tes_out.normal);

    vec4 c = vec4(1.0, -1.0, 0.0, 0.0) * normal.z +
             vec4(0.0, 0.0, 0.0, 1.0);

    g_FragColor = clamp(c, vec4(0.0), vec4(1.0));
}
