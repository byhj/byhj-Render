#version 430 core

layout (location = 0) out vec4 g_FragColor;

in VS_OUT 
{
   vec3 FragPos;
   vec3 Normal;
   vec2 TexCoord;
}vs_out;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
uniform vec3 u_LightPos = vec3(0.0f, 0.0f, 1.0f);
uniform vec3 u_ViewPos  = vec3(0.0f, 0.0f, 3.0f);
 
void main(void)
{
   vs_out.Normal = normalize(vs_out.Normal);

   vec3 diffuseColor = texture2D(texture_diffuse, vs_out.TexCoord).rgb;
   vec3 specularColor = texture2D(texture_specular, vs_out.TexCoord).rgb;

   //diffuse
   vec3 lightDir = u_LightPos - vs_out.FragPos;
   float diff = max(dot(lightDir, vs_out.Normal), 0.0f);
   vec3 diffuse = diff * diffuseColor;

   //specular
   vec3 reflectDir = reflect(-lightDir, vs_out.Normal);
   vec3 viewDir = u_ViewPos - vs_out.FragPos;
   vec3 halfDir = normalize(reflectDir + viewDir);
   float spec = pow(max(dot(halfDir, vs_out.Normal), 0.0), 32.0f);
   vec3 specular = spec * max(specularColor, 0.0f);

    g_FragColor = normalize( vec4(diffuse + specular, 1.0f) );

    g_FragColor = vec4(vec3(abs(vs_out.Normal.z)), 1.0f);
}
