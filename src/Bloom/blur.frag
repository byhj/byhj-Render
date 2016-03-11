#version 430 core

layout (location = 0) out vec4 g_fragColor;

in VS_OUT {
   vec2 texcoord;
}vs_out;

uniform sampler2D u_lightTex;
uniform bool u_horizontal ;
uniform float u_weights[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

void main()
{             
     vec2 tex_offset = 1.0 / textureSize(u_lightTex, 0); // gets size of single texel
     vec3 result = texture(u_lightTex, vs_out.texcoord).rgb * u_weights[0];

     if(u_horizontal)
     {
         for(int i = 1; i < 5; ++i)
         {
            result += texture(u_lightTex, vs_out.texcoord + vec2(tex_offset.x * i, 0.0)).rgb * u_weights[i];
            result += texture(u_lightTex, vs_out.texcoord - vec2(tex_offset.x * i, 0.0)).rgb * u_weights[i];
         }
     }
     else
     {
         for(int i = 1; i < 5; ++i)
         {
             result += texture(u_lightTex, vs_out.texcoord + vec2(0.0, tex_offset.y * i)).rgb * u_weights[i];
             result += texture(u_lightTex, vs_out.texcoord - vec2(0.0, tex_offset.y * i)).rgb * u_weights[i];
         }
     }

     g_fragColor = vec4(result, 1.0);
}