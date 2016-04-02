#ifndef Sphere_H
#define Sphere_H

#include <GL/glew.h>
#include "ogl/oglShader.h"
#include "ogl/oglUtility.h"
#include "modelMgr.h"

namespace byhj
{

class Sphere
{
public:
	Sphere();
	~Sphere();

	void init();
	void update();
	void render();
	void shutdown();

private:
	void init_buffer();
	void init_vertexArray();
	void init_shader();

	OGLShader CubeShader = "Sphere Shader" ;
	GLuint program ;
	GLuint vao;
	GLuint vbo;
	GLuint ibo;

	struct Uniform
	{
		GLuint model_loc;
		GLuint view_loc ;
		GLuint proj_loc ;
	}uniform;
 
};

}
#endif
