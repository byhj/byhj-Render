#ifndef Plane_H
#define Plane_H

#include <GL/glew.h>
#include "ogl/oglShader.h"
#include "ogl/oglUtility.h"

namespace byhj
{

class Plane
{
public:
	Plane();
	~Plane();

	void Init();
	void Render();
	void Shutdown();

private:
	void init_buffer();
	void init_vertexArray();
	void init_shader();
	void init_texture();

	Shader PlaneShader = { "Plane Shader" };
	GLuint program = byhj::OGL_VALUE;
	GLuint vao, ibo, vbo;
	GLuint tex_index = 0;

	struct Uniform
	{
		GLuint tex_loc = byhj::OGL_VALUE;
	}uniform;

};

}
#endif
