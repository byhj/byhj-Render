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
uniform float height_scale = 0.1f;

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{ 
    // number of depth layers
    const float minLayers = 10;
    const float maxLayers = 20;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));  
    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy / viewDir.z * height_scale; 
    vec2 deltaTexCoords = P / numLayers;
  
    // get initial values
    vec2  currentTexCoords     = texCoords;
    float currentDepthMapValue = texture(disTex, currentTexCoords).r;
      
    while(currentLayerDepth < currentDepthMapValue)
    {
        // shift texture coordinates along direction of P
        currentTexCoords -= deltaTexCoords;
        // get depthmap value at current texture coordinates
        currentDepthMapValue = texture(disTex, currentTexCoords).r;  
        // get depth of next layer
        currentLayerDepth += layerDepth;  
    }
    
    // -- parallax occlusion mapping interpolation from here on
    // get texture coordinates before collision (reverse operations)
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    // get depth after and before collision for linear interpolation
    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(disTex, prevTexCoords).r - currentLayerDepth + layerDepth;
 
    // interpolation of texture coordinates
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
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