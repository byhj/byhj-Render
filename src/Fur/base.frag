#version 410
 
 layout (location = 0) out vec4 g_fragColor;
 
 in VS_OUT
 {
     vec3 normal;
 } vs_out;
 

 void main(void)
 {    
    g_fragColor = vec4(vec3( abs(vs_out.normal.z) ), 1.0f);
 };