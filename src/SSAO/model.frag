#version 330 core

layout (location = 0) out vec4 g_posDepth;
layout (location = 1) out vec3 g_normal;
layout (location = 2) out vec4 g_color;

in VS_OUT {
   vec3 fragPos;
   vec3 normal;
   vec2 texcoord;
}vs_out;

const float NEAR = 0.1; // Projection matrix's near plane distance
const float FAR = 50.0f; // Projection matrix's far plane distance

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * NEAR * FAR) / (FAR + NEAR - z * (FAR - NEAR));	
}

void main()
{    
    // Store the fragment position vector in the first gbuffer texture
    g_posDepth.xyz = vs_out.fragPos;
    // And store linear depth into gPositionDepth's alpha component
    g_posDepth.a = LinearizeDepth(gl_FragCoord.z); // Divide by FAR if you need to store depth in range 0.0 - 1.0 (if not using floating point colorbuffer)

    // Also store the per-fragment normals into the gbuffer
    g_normal = normalize(vs_out.normal);
    // And the diffuse per-fragment color
    g_color.rgb = vec3(0.95); // Currently all objects have constant albedo color
}