#version 430 core

layout (location = 0) out vec4 g_FragColor;

in vec2 TexCoord;

uniform sampler2D textTex;
uniform vec3  textColor;

void main()
{
   vec4 texColor = vec4(1.0f, 1.0f, 1.0f, texture(textTex, TexCoord).r);
   g_FragColor =  texColor * vec4(textColor, 0.8f);

}

