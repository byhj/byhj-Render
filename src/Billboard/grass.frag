#version 330 core

in GS_OUT {
  vec2 texcoord;
}gs_out;

out vec4 color;

uniform sampler2D texture1;

void main()
{             
    vec2 tc = gs_out.texcoord;
	tc.y = 1.0f - tc.y;
    vec4 texColor = texture(texture1, tc);

	//alpha < 0.1 discard
    if(texColor.a < 0.1)
        discard;

    color = texColor;
}