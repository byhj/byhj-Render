#version 430 core

layout(location = 0) out float g_fragColor;

in VS_OUT {
  vec2 texcoord;
}vs_out;

uniform sampler2D u_posDepthTex;
uniform sampler2D u_normalTex;
uniform sampler2D u_noiseTex;

uniform vec3 samples[64];

// parameters (you'd probably want to use them as uniforms to more easily tweak the effect)
int kernelSize = 64;
float radius = 1.0;

// tile noise texture over screen based on screen dimensions divided by noise size
const vec2 noiseScale = vec2(1536.0f/4.0f, 864.0f/4.0f); 

uniform mat4 u_proj;

void main()
{
    // Get input for SSAO algorithm
    vec3 fragPos = texture2D(u_posDepthTex, vs_out.texcoord).xyz;
    vec3 normal  = texture2D(u_normalTex, vs_out.texcoord).rgb;
    vec3 randomVec = texture2D(u_noiseTex, vs_out.texcoord * noiseScale).xyz;

    // Create TBN change-of-basis matrix: from tangent-space to view-space
    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);

    // Iterate over the sample kernel and calculate occlusion factor
    float occlusion = 0.0;
    for(int i = 0; i < kernelSize; ++i)
    {
        // get sample position
        vec3 sampleT = TBN * samples[i]; // From tangent to view-space
        sampleT = fragPos + sampleT * radius; 
        
        // project sample position (to sample texture) (to get position on screen/texture)
        vec4 offset = vec4(sampleT, 1.0);
        offset = u_proj * offset; // from view to clip-space
        offset.xyz /= offset.w; // perspective divide
        offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0
        
        // get sample depth
        float sampleDepth = -texture(u_posDepthTex, offset.xy).w; // Get depth value of kernel sample
        
        // range check & accumulate
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth ));
        occlusion += (sampleDepth >= sampleT.z ? 1.0 : 0.0) * rangeCheck;           
    }
    occlusion = 1.0 - (occlusion / kernelSize);
    
    g_fragColor = occlusion;
}