#version 430 core

in VS_OUT {
  vec2 texcoord;
  vec3 lightPos;
  vec3 viewPos;
  vec3 fragPos;
}vs_out;

layout (location = 0) out vec4 g_FragColor;

uniform sampler2D colorTex;
uniform sampler2D normalTex;
uniform sampler2D disTex;
uniform float height_scale = 0.01f;

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{ 
    float height =  texture(disTex, texCoords).r;     
    return texCoords - viewDir.xy / viewDir.z * (height * height_scale);            
}

void main(void)
{
   vec3 viewDir = normalize(vs_out.viewPos - vs_out.fragPos);
   vec2 texCoords = vs_out.texcoord;
   texCoords = ParallaxMapping(vs_out.texcoord, viewDir);

   vec3 normal = texture2D(normalTex, texCoords).rgb;
   normal = normalize(normal * 2.0f - 1.0f);    
   vec3 color = texture2D(colorTex, texCoords).rgb;
   // Ambient
    vec3 ambient = 0.1f * color;
    // Diffuse
    vec3 lightDir = normalize(vs_out.lightPos - vs_out.fragPos);
    float diff = max(dot(lightDir, normal), 0.0f);
    vec3 diffuse = diff * color;
	    // Specular

    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    vec3 specular = vec3(0.2) * spec;
    
    g_FragColor  = vec4(ambient + diffuse + specular, 1.0f);
}