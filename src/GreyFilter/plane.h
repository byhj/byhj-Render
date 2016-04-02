#ifndef Plane_H
#define Plane_H

#include "ogl/oglShader.h"
#include "ogl/oglUtility.h"

namespace byhj
{

class Plane
{
public:
	Plane();
	~Plane();

	void init();
	void update();
	void render();
	void shutdown();

private:
	void init_buffer();
	void init_vertexArray();
	void init_shader();
	void init_texture();

	OGLShader PlaneShader =  "Plane Shader" ;
	GLuint program;
	GLuint vao, ibo, vbo;
	GLuint tex_index = 0;

	GLuint tex_loc;

};

}
#endif
