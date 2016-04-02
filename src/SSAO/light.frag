#version 430 core

layout (location = 0) out vec4 g_fragColor;

in VS_OUT {
 vec2 texcoord;
}vs_out;

layout (binding = 0) uniform sampler2D u_posDepthTex;
layout (binding = 1) uniform sampler2D u_normalTex;
layout (binding = 2) uniform sampler2D u_colorTex;
layout (binding = 3) uniform sampler2D u_ssaoTex;

struct Light {
    vec3 Position;
    vec3 Color;
    
    float Linear;
    float Quadratic;
};
uniform Light light;
uniform vec3 u_viewPos;

void main()
{             
    // Retrieve data from g-buffer
    vec3 FragPos = texture(u_posDepthTex, vs_out.texcoord).rgb;
    vec3 Normal  = texture(u_normalTex,   vs_out.texcoord).rgb;
    vec3 Diffuse = texture(u_colorTex,    vs_out.texcoord).rgb;
    float AmbientOcclusion = texture(u_ssaoTex, vs_out.texcoord).r;
    
    // Then calculate lighting as usual
    vec3 ambient = vec3(0.5 * AmbientOcclusion); // <-- this is where we use ambient occlusion
    vec3 lighting  = ambient; 
    vec3 viewDir  = normalize(u_viewPos-FragPos); // Viewpos is (0.0.0)

    // Diffuse
    vec3 lightDir = normalize(light.Position - FragPos);
    vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * light.Color;

    // Specular
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(Normal, halfwayDir), 0.0), 8.0);
    vec3 specular = light.Color * spec;

    // Attenuation
    float distance = length(light.Position - FragPos);
    float attenuation = 1.0 / (1.0 + light.Linear * distance + light.Quadratic * distance * distance);
    diffuse *= attenuation;
    specular *= attenuation;
    lighting += diffuse + specular;

    g_fragColor = vec4(lighting, 1.0);
}