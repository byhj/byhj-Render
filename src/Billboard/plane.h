#ifndef Plane_H
#define Plane_H

#include "ogl/oglShader.h"
#include "ogl/oglUtility.h"

#include <glfw/glfw3.h>

namespace byhj
{

	class Plane
	{
	public:
		Plane(): program(0), vao(0), vbo(0), PlaneShader("Plane Shader") {}
		~Plane() {}

	public:
		void Init();
		void Render(const OGLMVPMatrix &matrix);
		void Shutdown();

	private:
		void init_shader();
		void init_buffer();
		void init_vertexArray();
		void init_texture();

		GLuint vao, vbo, program;
		GLuint texture, tex_loc;
		GLuint model_loc, view_loc, proj_loc;
		OGLShader PlaneShader;			
	};

}

#endif