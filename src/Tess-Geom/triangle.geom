#version 430 core

//Gemo run each primitive
layout (triangles) in ;
layout (triangle_strip, max_vertices = 6) out;

in TES_OUT
{
  vec4 color;
}tes_out[];

out GEOM_OUT
{
   vec4 color;
}geom_out;

void main(void)
{
   //Input one triangle, but we use geometry shader to output two triangle

   //First Triangle, end by primitive
   for (int i = 0; i != gl_in.length(); ++i)
   {
      geom_out.color = tes_out[i].color;
      gl_Position = gl_in[i].gl_Position;
	  EmitVertex();		
   }
   EndPrimitive();

   //Second Triangle
   for (int i = 0; i != gl_in.length(); ++i)
   {
      geom_out.color = tes_out[i].color;
      gl_Position =  gl_in[i].gl_Position * vec4(-1.0f, -1.0f, 1.0f, 1.0f);
	  EmitVertex();		
   }
   EndPrimitive();



}