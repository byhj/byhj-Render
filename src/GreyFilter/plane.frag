#version 430 core

layout (location = 0) out vec4 fragColor;

in VS_OUT {
  vec2 tc;
}fs_in;

 uniform sampler2D tex;

void main(void)
{
    vec4 color;
    color = texture(tex, fs_in.tc);

   	if (fs_in.tc.s >= 0.5)
	{
		float grey = color.r*0.299 + color.g*0.587 + color.b*0.114;
		
		fragColor = vec4(grey, grey, grey, 1.0f);
	}
	else
	{
		fragColor = color;
	}

}