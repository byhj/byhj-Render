#version 430 core

layout (triangles) in;
layout (line_strip, max_vertices = 4) out;

in VS_OUT {
  vec3 normal;
  vec2 texcoord;
}vs_out[];

out GEOM_OUT {
   vec3 normal;
   vec2 texcoord;
}geom_out;

uniform float fur_length = 0.003f;

void main(void)
{
   //Calc the face normal                                                      
    vec3 ab = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;     
    vec3 ac = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;     
    vec3 face_normal = normalize(cross(ab, ac));   
	
	//Calc the triangle centroid   
	vec4 tri_centroid = (gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position) / 3.0;   


	gl_Position =  tri_centroid;                           
    EmitVertex();                                                  
                                                                                                                           
    gl_Position =  (tri_centroid + vec4(face_normal * fur_length, 0.0));                                      
    EmitVertex();                                                      
    EndPrimitive();                                                    
           
		   
		                                                               
    gl_Position = gl_in[0].gl_Position;                                                             
    EmitVertex();                                                      
                                                                       
    gl_Position = (gl_in[0].gl_Position + vec4(vs_out[0].normal * fur_length, 0.0));                                    
    EmitVertex();                                                      
    EndPrimitive();      
}