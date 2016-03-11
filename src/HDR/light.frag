#version 430 core

out vec4 FragColor;

in VS_OUT 
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
} vs_out;

struct Light 
{
    vec3 Position;
    vec3 Color;
};

uniform vec3 viewPos;
uniform sampler2D diffuseTexture;
uniform Light lights[4];


void main()
{           

    vec3 color = texture(diffuseTexture, vs_out.TexCoord).rgb;

    vec3 normal = normalize(vs_out.Normal);

    // Ambient
    vec3 ambient = 0.0 * color;

    // Lighting
    vec3 lighting = vec3(0.0f);
    for(int i = 0; i < 4; i++)
    {
        // Diffuse
        vec3 lightDir = normalize(lights[i].Position - vs_out.FragPos);
        float diff = max(dot(lightDir, normal), 0.0);
        vec3  diffuse = lights[i].Color * diff * color;      
        vec3  result = diffuse;     
		   
        // Attenuation (use quadratic as we have gamma correction)
        float dis = length(vs_out.FragPos - lights[i].Position);
        result *= 1.0 / (dis * dis);
        lighting += result;       
    }

    FragColor = vec4(ambient + lighting, 1.0f);
}