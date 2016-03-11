#version 430 core

in vec2 TexCoords;
out vec4 color;

uniform sampler2D tex;

float LinearizeDepth(float depth)
{
    float near = 0.1; 
    float far = 100.0; 
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near) / (far + near - z * (far - near));	
}

void main()
{             
    color = texture(tex, TexCoords);
   // color = vec4(vec3(gl_FragCoord.z), 1.0f);
      // float depth = LinearizeDepth(gl_FragCoord.z);
      // color = vec4(vec3(depth), 1.0f);
}