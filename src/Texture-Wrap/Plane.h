#ifndef Plane_H
#define Plane_H

#include "ogl/oglShader.h"
#include "utility.h"

namespace byhj {

	class Plane {
	public:
		Plane();
		~Plane();

		void init();
		void update();
		void render();
		void shutdown();


		void init_buffer();
		void init_vertexArray();
		void init_texture();
		void init_shader();

	private:
		GLuint m_vao = OGL_ONE; 
		GLuint m_vbo = OGL_ONE;
		GLuint m_ibo = OGL_ONE;
		GLuint m_tex = OGL_ONE;
		GLuint m_program = OGL_ONE;
		GLuint m_offsetLoc;
		GLuint tex;
		OGLShader PlaneShader = "Plane Shader";
	};
}
#endif