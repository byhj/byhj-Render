#ifndef Planet_H
#define Planet_H


#include <gl/glew.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ogl/oglShader.h"

namespace byhj
{
	class Planet
	{
	public:

		void init();
		void update();
		void render();
		void shutdown();

	private:
		void init_buffer();
		void init_shader();

		struct UniformLoc
		{
			GLuint model;
			GLuint view;
			GLuint proj;
		}uniform_loc;

		GLuint m_vao;
		GLfloat m_Aspect = 1.5f;
		OGLShader m_PlanetShader = "Planet Shader";
		GLuint m_Program;
	};
}
#endif