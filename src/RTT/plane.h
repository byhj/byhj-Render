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
		Plane() = default;
		~Plane() = default;

	public:
		void init();
		void render(const glm::mat4 &mvp);
		void update();
		void shutdown();


	private:
		void init_shader();
		void init_buffer();
		void init_vertexArray();
		void init_texture();
		
		GLuint m_vao, m_vbo, m_program;
		GLuint m_texture, tex_loc;
		GLuint mvp_loc;
		OGLShader PlaneShader = "Plane Shader";			
	};

}

#endif