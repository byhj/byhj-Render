#version 330 core

layout(location = 0) out vec4 fragColor;

in vec2 tc;

uniform sampler2D hdrBuffer;
uniform bool hdr = true;
uniform float exposure;

void main()
{             
    const float gamma = 2.2;
	//Float texture color
    vec3 hdrColor = texture(hdrBuffer, tc).rgb;

    if(hdr)
    {
        // reinhard
        // vec3 result = hdrColor / (hdrColor + vec3(1.0));
        // exposure tone mapping back to [0.0f, 1.0f]
        vec3 result = vec3(1.0) - exp(-hdrColor * exposure);

        // also gamma correct while we're at it       
        result = pow(result, vec3(1.0 / gamma));
        fragColor  = vec4(result, 1.0f);
    }
    else
    {
        vec3 result = pow(hdrColor, vec3(1.0 / gamma));
        fragColor = vec4(result, 1.0);
    }
}