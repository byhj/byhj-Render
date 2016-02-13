#ifndef Point_H
#define Point_H

#include <gl/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ogl/oglShader.h"
#include "utility.h"

namespace byhj
{
	class Point 
	{
	public:

	Point() ;
	~Point();

	void init();
	void update();
	void render();
	void shutdown();

	private:

		void init_buffer();
		void init_vertexArray();
		void init_shader();

		GLuint m_program  = OGL_ONE;
		GLuint m_vao  = OGL_ONE;
		GLuint m_ibo  = OGL_ONE;
		GLuint m_vbo  = OGL_ONE;
		GLuint u_model;
		GLuint u_view;
		GLuint u_proj;
		GLuint time_loc;
		GLuint texId;
		OGLShader m_PointShader = "Point Shader";
	};
}
#endif