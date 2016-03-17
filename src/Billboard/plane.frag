#version 430 core

layout(location = 0) out vec4 fragColor;

in vec2 TexCoords;

uniform sampler2D tex;

void main()
{             
    fragColor = texture(tex, TexCoords);
}