#version 430 core

layout (location = 0) out vec4 g_FragColor;

uniform sampler2D posTex;
uniform sampler2D normalTex;
uniform sampler2D colorTex;


in VS_OUT 
{
   vec2 TexCoord;
}vs_out;
 
void main(void)
{
    vec3 fragPos = texture(posTex, vs_out.TexCoord).rgb;
    vec3 normal  = texture(normalTex, vs_out.TexCoord).rgb;
    vec3 color   = texture(colorTex, vs_out.TexCoord).rgb;
    
	g_FragColor = vec4(color, 1.0);
}