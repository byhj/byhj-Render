#version 430 core

layout (triangles, invocations = 4) in;
layout (triangle_strip, max_vertices = 3) out;

layout (std140, binding = 0) uniform TransformBlock 
{
  mat4 mvps[4];
};

in VS_OUT {
  vec4 color;
}vs_out[];

out GEOM_OUT {
  vec4 color;
}geom_out;

void main(void)
{
   for (int i = 0; i < gl_in.length(); ++i) {
       geom_out.color = vs_out[i].color;
	   gl_Position = mvps[gl_InvocationID] * gl_in[i].gl_Position;

	   //set viewport
	   gl_ViewportIndex = gl_InvocationID;
	   EmitVertex();
   }
   EndPrimitive();
}