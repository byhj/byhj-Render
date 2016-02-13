#ifndef Triangle_H
#define Triangle_H

#include <gl/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ogl/oglShader.h"
#include "utility.h"

namespace byhj
{
	class Triangle 
	{
	public:

	Triangle() ;
	~Triangle();

	void init();
	void update();
	void render();
	void shutdown();

	private:

		void init_buffer();
		void init_vertexArray();
		void init_shader();

		GLuint m_vert, m_frag, m_pipe;
		GLuint m_vao  = OGL_ONE;
		GLuint m_ibo  = OGL_ONE;
		GLuint m_vbo  = OGL_ONE;
		GLuint mvp_loc;
		GLuint color_loc;
	};
}
#endif