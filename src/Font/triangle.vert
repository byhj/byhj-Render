#version 430 core

layout (location = 0) in vec3 g_Position;

out VS_OUT {
  vec4 color;
}vs_out;

void main()
{

   vs_out.color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
   gl_Position = vec4(g_Position, 1.0f);

}