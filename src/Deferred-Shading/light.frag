#version 430 core

layout (location = 0) out vec4 g_fragColor;

uniform sampler2D posTex;
uniform sampler2D normalTex;
uniform sampler2D colorTex;
uniform vec3 u_viewPos = vec3(0.0f, 0.0f, 5.0f);
uniform vec3 u_lightPos = vec3(0.0f, 1.0f, 3.0f);

in VS_OUT 
{
   vec2 TexCoord;
}vs_out;
 
void main(void)
{
    vec3 fragPos = texture(posTex, vs_out.TexCoord).rgb;
    vec3 normal  = texture(normalTex, vs_out.TexCoord).rgb;
    vec3 color   = texture(colorTex, vs_out.TexCoord).rgb;
    
    vec3 viewDir  = normalize(u_viewPos - fragPos);
    vec3 lightDir = normalize(u_lightPos - fragPos);
    vec3 diffuse = max(dot(normal, lightDir), 0.0) * color;

    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    vec3 specular = spec * color;

    g_fragColor = vec4(diffuse + specular , 1.0);
}