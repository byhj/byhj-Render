#version 430 core

layout (location = 0) in vec4 g_Position; // <vec2 pos, vec2 tex>
out vec2 TexCoord;

uniform mat4 mvp;

void main()
{
  gl_Position = mvp * vec4(g_Position.xy, 0.0f, 1.0f);
  TexCoord = g_Position.zw;
}
