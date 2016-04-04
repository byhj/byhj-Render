#version 430 core

layout (location = 0) out vec4 g_FragColor;

in TES_OUT
{
   vec3 fragPos;
   vec3 normal;
   vec2 texcoord;
}tes_out;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
uniform vec3 u_LightPos = vec3(0.0f, 0.0f, 1.0f);
uniform vec3 u_camPos  = vec3(0.0f, 0.0f, 5.0f);
 
void main(void)
{
   normalize(tes_out.normal);
   normalize(tes_out.fragPos);
   vec3 ambient = vec3(0.01);
   vec3 diffuseColor = texture2D(texture_diffuse, tes_out.texcoord).rgb;
   vec3 specularColor = texture2D(texture_specular, tes_out.texcoord).rgb;

   //diffuse
   vec3 lightDir = normalize(u_LightPos - tes_out.fragPos);
   float diff = max(dot(lightDir, tes_out.normal), 0.0f);
   vec3 diffuse = diff * diffuseColor;

   //specular
   vec3 reflectDir = reflect(-lightDir, tes_out.normal);
   vec3 viewDir = normalize(u_camPos - tes_out.fragPos);
   vec3 halfDir = normalize(reflectDir + viewDir);
   float spec = pow(max(dot(halfDir, tes_out.normal), 0.0), 32.0f);
   vec3 specular = spec * max(specularColor, 0.0f);

   // g_FragColor = vec4(diffuse + specular + ambient, 1.0f);
	g_FragColor = vec4( abs(tes_out.normal.z));
}
