#version 430 core

layout (location = 0) out vec4 g_FragColor;

const int LIGHTNUMS = 4;

in VS_OUT
{
  vec3 FragPos;
  vec3 Normal;
  vec2 TexCoord;
}vs_out;

uniform sampler2D u_WoodTex;

uniform vec3 u_LightPos[LIGHTNUMS];
uniform vec3 u_LightColor[LIGHTNUMS];
uniform vec3 u_ViewPos;
uniform bool u_Gamma = false;

subroutine vec3 LightModelSub(vec3 normal, vec3 fragPos, vec3 lightPos, vec3 lightColor);

subroutine(LightModelSub) vec3 Phong(vec3 normal, vec3 fragPos, vec3 lightPos, vec3 lightColor)
{
  //Diffuse
  vec3 lightDir = normalize(lightPos - fragPos);
  float diff    = max(0.0f, dot(lightDir, normal) );
  vec3 diffuse  = diff * lightColor;

  //Specular
  vec3 viewDir = normalize(u_ViewPos - fragPos);
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(reflectDir, normal), 0.0f), 64.0f);
  vec3 specular = spec * lightColor;

  //Attenuation
  float max_dis = 1.5;
  float dis = length(lightPos - fragPos);
  float att= 1.0 / (u_Gamma ? dis * dis : dis);

  diffuse *= att;
  specular *= att;

  vec3 light = diffuse + specular;

  return light; 

}

subroutine (LightModelSub) vec3 BlinnPhong(vec3 normal, vec3 fragPos, vec3 lightPos, vec3 lightColor)
{
  vec3 lightDir = normalize(lightPos - fragPos);
  float diff    = max(0.0f, dot(lightDir, normal) );
  vec3 diffuse  = diff * lightColor;

  //Specular
  vec3 viewDir = normalize(u_ViewPos - fragPos);
  vec3 halfDir = normalize(lightDir + viewDir);

  float spec = pow(max(dot(halfDir, normal), 0.0f), 64.0f);
  vec3 specular = spec * lightColor;

  //Attenuation
  float max_dis = 1.5;
  float dis = length(lightPos - fragPos);
  float att = 1.0 / (u_Gamma ? dis * dis : dis);

  diffuse *= att;
  specular *= att;

  vec3 light = diffuse + specular;

  return light; 

}

subroutine uniform LightModelSub lightModelUniform;

void main()
{
  vec4 texColor = texture(u_WoodTex, vs_out.TexCoord);
  vec3 light = vec3(0.0f);

  for (int i = 0; i < LIGHTNUMS; ++i)
      light += lightModelUniform(vs_out.Normal, vs_out.FragPos, u_LightPos[i], u_LightColor[i]);
  
  vec4 pixelColor = texColor * vec4(light, 1.0f);

   if(u_Gamma)
        pixelColor = pow(pixelColor, vec4(1.0/2.2) );

  g_FragColor = pixelColor;
}
