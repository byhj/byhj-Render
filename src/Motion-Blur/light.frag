#version 430 core

layout (location = 0) out vec4 g_fragColor;

uniform sampler2D motionVec;
uniform sampler2D colorTex;


uniform vec3 u_viewPos = vec3(0.0f, 0.0f, 5.0f);
uniform vec3 u_lightPos = vec3(0.0f, 1.0f, 3.0f);

in VS_OUT 
{
   vec2 TexCoord;
}vs_out;
 
void main(void)
{
    vec2 motion  = texture(motionVec, vs_out.TexCoord).xy / 2.0f;
 
    vec2 texCoord = vs_out.TexCoord;
	vec4 Color = vec4(0.0f);

    Color += texture(colorTex, texCoord) * 0.4;
    texCoord -= motion;
    Color += texture(colorTex, texCoord) * 0.3;
    texCoord -= motion;
    Color += texture(colorTex, texCoord) * 0.2;
    texCoord -= motion;
    Color += texture(colorTex, texCoord) * 0.1;

    g_fragColor = Color;
}