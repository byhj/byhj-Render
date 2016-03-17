#version 330 core

in GS_OUT {
  vec2 texcoord;
  vec3 color;
}gs_out;

out vec4 g_fragColor;

uniform sampler2D texture1;

void main()
{             
    vec2 tc = gs_out.texcoord;
    vec4 texColor = texture(texture1, tc);

    if(texColor.r == 0.0f && texColor.g == 0.0f && texColor.b == 0.0f) {
	   discard;
	}

    g_fragColor = texColor * vec4(gs_out.color, 1.0f);
}