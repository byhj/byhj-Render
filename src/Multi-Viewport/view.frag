#version 430 core                                              
                                                               
layout (location = 0) out vec4 g_FragColor;                                               
                                                               
in GEOM_OUT                                                      
{                                                                                                            
    vec4 color;                                                
} geom_out;                                                       
                                                               
void main(void)                                                
{                                                              
    g_FragColor =  geom_out.color;         
}   