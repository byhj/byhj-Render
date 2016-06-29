#ifndef Triangle_H
#define Triangle_H

#include <gl/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ogl/oglShader.h"

namespace byhj
{
	class Cube
	{
	public:

	Cube()  = default;
	~Cube() = default;

	void init();
	void update();
	void render();
	void shutdown();

	private:

		void init_buffer();
		void init_vertexArray();
		void init_shader();

	    GLuint m_program;
	    GLuint m_vao;
	    GLuint m_ibo;
	    GLuint m_vbo;

	   OGLShader m_CubeShader ={ "CubeShader" };
	};
}
#endif