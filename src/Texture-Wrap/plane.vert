#version 430 core

out VS_OUT {
  vec2 texcoord;
}vs_out;

uniform vec2 u_offset;

const vec4 VertexData[] = vec4[](vec4(-0.45, -0.45, 0.5, 1.0),               
                                 vec4( 0.45, -0.45, 0.5, 1.0),               
                                 vec4(-0.45,  0.45, 0.5, 1.0),               
                                 vec4( 0.45,  0.45, 0.5, 1.0) );              
                                                                    

void main(void)
{
    gl_Position = VertexData[gl_VertexID] + vec4(u_offset, 0.0, 0.0);   
	//Set the texCoord more than 1, so we can use the texture wrap          
    vs_out.texcoord   = VertexData[gl_VertexID].xy * 3.0 + vec2(0.45 * 3);     
}
