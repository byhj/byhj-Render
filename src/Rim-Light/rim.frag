#version 430 core

layout (location = 0) out vec4 g_FragColor;

in VS_OUT 
{
   vec3 FragPos;
   vec3 Normal;
   vec2 TexCoord;
}vs_out;

uniform vec3 u_ambient;
uniform vec3 diffuse_albedo = vec3(0.5, 0.2, 0.7);
uniform vec3 specular_albedo = vec3(0.7);
uniform float specular_power = 128.0;
uniform vec3 ambient = vec3(0.1);
uniform vec3 rim_color = vec3(0.1, 0.7, 0.2);
uniform float rim_power = 5.0;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
uniform vec3 u_LightPos = vec3(0.0f, 1.0f, 1.0f);
uniform vec3 u_ViewPos  = vec3(0.0f, 0.0f, 3.0f);

 vec3 calculate_rim(vec3 N, vec3 V) // cal the rim light
{
    float f = 1.0 - dot(N, V);

    f = smoothstep(0.0, 1.0, f);
    f = pow(f, rim_power);

    return f * rim_color;
}

void main(void)
{
   vs_out.Normal = normalize(vs_out.Normal);

   vec3 diffuseColor = texture2D(texture_diffuse, vs_out.TexCoord).rgb;
   vec3 specularColor = texture2D(texture_specular, vs_out.TexCoord).rgb;

   vec3 N = normalize(vs_out.Normal);
   vec3 L = normalize(u_LightPos - vs_out.FragPos);
   vec3 V = normalize(u_ViewPos);

   vec3 R = reflect(-L, N);
   vec3 diffuse = max(dot(N, L), 0.0) * diffuse_albedo;
   vec3 specular = pow( max(dot(R, V), 0.0), specular_power ) * specular_albedo;
   vec3 rim = calculate_rim(N, V);

    g_FragColor = normalize( vec4(diffuse + specular + rim, 1.0f) );

   // g_FragColor = vec4(vec3(abs(vs_out.Normal.z)), 1.0f) ;
}
