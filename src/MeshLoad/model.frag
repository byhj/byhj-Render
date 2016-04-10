#version 430 core

layout (location = 0) out vec4 g_FragColor;

in VS_OUT 
{
  vec2 texcoord;
  vec3 fragPos;
  vec3 tangentLightPos;
  vec3 tangentViewPos;
  vec3 tangentFragPos;
}vs_out;

uniform sampler2D u_diffuseTex;
uniform sampler2D u_specularTex;
uniform sampler2D u_normalTex;

 uniform vec3 u_lightPos = vec3(0.0f, 20.0f, 15.0f);

void main(void)
{

// Local normal, in tangent space. V tex coordinate is inverted because normal map is in TGA (not in DDS) for better quality
   vec2 texcoord = vec2(vs_out.texcoord.x, -vs_out.texcoord.y);
   vec3 normal  = texture2D(u_normalTex, texcoord).rgb;
   normal = (2.0f * normal) - 1.0f;

   vec3 diffuseColor = texture2D(u_diffuseTex, vs_out.texcoord).rgb;
   vec3 specularColor = texture2D(u_normalTex, vs_out.texcoord).rgb;
   vec3 ambient = vec3(0.2f) * diffuseColor;

   //diffuse
   vec3 lightDir = normalize(vs_out.tangentLightPos - vs_out.tangentFragPos);
   float diff = max(dot(lightDir, normal), 0.0f);
   vec3 diffuse = diff * diffuseColor;

   //specular
   vec3 reflectDir = reflect(-lightDir, normal);
   vec3 viewDir = normalize( vs_out.tangentViewPos -  vs_out.tangentFragPos);
   vec3 halfDir = normalize(reflectDir + viewDir);
   float spec = pow(max(dot(halfDir, normal), 0.0), 32.0f);
   vec3 specular = vec3(0.2f) * diffuseColor;

    float max_dis = 1.5;
    float dis = length(u_lightPos - vs_out.fragPos);
    float att = 1.0 / dis * dis;

    g_FragColor = vec4( (diffuse + specular) * att + ambient, 1.0f); //vec4( (diffuse + specular) * att + ambient, 1.0f);
	//g_FragColor = vec4( abs(vs_out.Normal.z));
}
