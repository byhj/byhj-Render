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
uniform float u_LightCutOff[LIGHTNUMS];
uniform float u_LightOuterCutOff[LIGHTNUMS];

uniform vec3 u_ViewPos;
uniform bool u_Gamma = true;

vec3 BlinnPhong(vec3 normal, vec3 fragPos, vec3 lightPos, vec3 lightColor);

void main()
{
  vec4 texColor = texture(u_WoodTex, vs_out.TexCoord);
  vec3 light = vec3(0.01f);
  for (int i = 0; i != 4; ++i) {
  //your should normalize the lightDir, then dot(A, B) = cos(A, B)
        vec3 lightDir = normalize(u_LightPos[i] - vs_out.FragPos);
        float theta = dot(lightDir, normalize(vec3(0.0f, 1.0f, 0.0f)));
		float epsilon = (u_LightCutOff[i] - u_LightOuterCutOff[i]);
        float intensity = clamp((theta - u_LightOuterCutOff[i]) / epsilon, 0.0, 1.0);

         light += BlinnPhong(vs_out.Normal, vs_out.FragPos, u_LightPos[i], u_LightColor[i]) * intensity;
   }
  vec4 pixelColor = texColor * vec4(light + vec3(0.2f, 0.2f, 0.2f), 1.0f);

   if(u_Gamma)
        pixelColor = pow(pixelColor, vec4(1.0/2.2) );

  g_FragColor = pixelColor ;
}



vec3 BlinnPhong(vec3 normal, vec3 fragPos, vec3 lightPos, vec3 lightColor)
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

