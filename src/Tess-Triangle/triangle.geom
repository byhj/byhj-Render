#version 430 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in TE_OUT
{
  vec3 Pos;
  vec3 PatchDistance;
}te_out[3];

out GE_OUT
{
  vec3 FacetNormal;
  vec3 PatchDistance;
  vec3 TriDistance;
}ge_out;

uniform mat4 model;
uniform mat4 view;

void main(void)
{
   mat3 normal_mat = mat3(transpose(inverse(view * model) ) );
   vec3 A = te_out[2].Pos - te_out[0].Pos;
   vec3 B = te_out[1].Pos - te_out[0].Pos;
   ge_out.FacetNormal = normal_mat * normalize(cross(A, B));

   ge_out.PatchDistance = te_out[0].PatchDistance;
   ge_out.TriDistance  = vec3(1, 0, 0);
   gl_Position = gl_in[0].gl_Position;
   EmitVertex();

   ge_out.PatchDistance = te_out[1].PatchDistance;
   ge_out.TriDistance  = vec3(0, 1, 0);
   gl_Position = gl_in[1].gl_Position;
   EmitVertex();

   ge_out.PatchDistance = te_out[2].PatchDistance;
   ge_out.TriDistance  = vec3(0, 0, 1);
   gl_Position = gl_in[2].gl_Position;
   EmitVertex();

   EndPrimitive();
}


