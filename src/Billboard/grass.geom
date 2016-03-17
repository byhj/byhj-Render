#version 430 core

layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform vec3 camPos;

in VS_OUT {
  vec3 color;
}vs_out[];

out GS_OUT {
  vec2 texcoord;
  vec3 color;
}gs_out;

void main(void) 
{
    vec3 pos = gl_in[0].gl_Position.xyz;
	vec3 viewPos = normalize(camPos - pos);
	vec3 up = vec3(0.0f, 1.0f, 0.0f);
	vec3 right = cross(viewPos, up);
	
	//pos is the bottom middle Polygon
	mat4 vp = proj * view * model;
	pos -= (right * 0.5f);
	pos.y += 1.0f;
	gl_Position = vp * vec4(pos, 1.0f);
	gs_out.texcoord = vec2(0.0f, 0.0f);
	gs_out.color = vs_out[0].color;
	EmitVertex();

	pos.y -= 1.0f;
	gl_Position = vp * vec4(pos, 1.0f);
	gs_out.texcoord = vec2(0.0f, 1.0f);
	gs_out.color = vs_out[0].color;
	EmitVertex();

	pos += right;
	gl_Position = vp * vec4(pos, 1.0f);
	gs_out.texcoord = vec2(1.0f, 1.0f);
	gs_out.color = vs_out[0].color;
	EmitVertex();

	pos.y += 1.0f;
	gl_Position = vp * vec4(pos, 1.0f);
	gs_out.texcoord = vec2(1.0f, 0.0f);
	gs_out.color = vs_out[0].color;
	EmitVertex();

	EndPrimitive();
}