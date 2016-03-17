#version 330 core

in GS_OUT {
  vec2 texcoord;
}gs_out;

out vec4 color;

uniform sampler2D texture1;

void main()
{             
    vec2 tc = gs_out.texcoord;
    vec4 texColor = texture(texture1, tc);

    if(texColor.r == 0.0f && texColor.g == 0.0f && texColor.b == 0.0f) {
	   discard;
	}

    color = texColor;
}